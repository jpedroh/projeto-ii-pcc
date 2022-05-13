#include <getopt.h>
#include <iostream>
#include <vector>
#include <glob.h>
#include <fstream>
#include <iterator>
#include <memory>
#include <algorithm>
#include <sstream>
#include <utility>
#include "sarr.cpp"

using namespace std;

struct ipmt_options
{
    string pattern_file;
    bool is_count;
    bool is_help;
    string pattern;
    string action;
    vector<string> files;
};

vector<string> parse_files_from_glob(string pattern)
{
    glob_t globbuf;
    int err = glob(pattern.c_str(), 0, NULL, &globbuf);
    vector<string> files;
    if (err == 0)
    {
        for (size_t i = 0; i < globbuf.gl_pathc; i++)
        {
            files.emplace_back(globbuf.gl_pathv[i]);
        }

        globfree(&globbuf);
    }

    return files;
}

ipmt_options ProcessArgs(int argc, char **argv)
{
    const char* const short_opts = "p:ch";
    const option long_opts[] = {
        {"pattern", required_argument, nullptr, 'p'},
        {"count", no_argument, nullptr, 'c'},
        {"help", no_argument, nullptr, 'h'}};
    struct ipmt_options options = {"", false, false, "", ""};

    options.action = argv[1];
    int args = 2;

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        if (-1 == opt)
        {
            break;
        }

        switch (opt)
        {
        case 'p':
            args += 2;
            options.pattern_file = optarg;
            break;
        case 'c':
            args += 1;
            options.is_count = true;
            break;
        case 'h':
        case '?':
        default:
            options.is_help = true;
            break;
        }
    }

    if ((options.pattern_file == "") && (options.action == "search"))
    {
        options.pattern = argv[args];
        args++;
    }

    for (int i = args; i < argc; i++)
    {
        auto files = parse_files_from_glob(argv[i]);
        options.files.insert(options.files.end(), files.begin(), files.end());
    }

    return options;
}

vector<string> get_patterns_from_options(ipmt_options options)
{
    vector<string> patterns;
    if (options.pattern_file == "")
    {
        patterns.emplace_back(options.pattern);
    }
    else
    {
        ifstream file(options.pattern_file);
        string line;
        for (string line; getline(file, line);)
        {
            patterns.emplace_back(line);
        }
    }
    return patterns;
}

void PrintHelp()
{
    cout << "--help: Mostra esta mensagem de ajuda\n";
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        PrintHelp();
        return 1;
    }

    auto options = ProcessArgs(argc, argv);

    if (options.is_help)
    {
        PrintHelp();
        return 1;
    }

    //TODO: melhorar esse pattern
    if(options.action == "index") {
        Sarr algorithm;

        for(auto file_name : options.files)
        {
            string line, aux_file_name = file_name;
            
            int idx = aux_file_name.find(".");
            aux_file_name = aux_file_name.replace(idx, file_name.size() - idx, ".idx");

            ifstream file(file_name);
            FILE *indexed_file = fopen(aux_file_name.c_str(), "wb");

            while(getline(file, line)){
                algorithm.index(line, indexed_file);
            }

            fclose(indexed_file);
            file.close();
        }
    } else if(options.action == "search"){
        Sarr algorithm;
        auto patterns = get_patterns_from_options(options);
        long long total_occ = 0;
        vector<vector<string>> total_lines_occ;

        for(auto file_name : options.files)
        {
            auto lines_occurrance = algorithm.search(patterns, file_name);
            total_occ += lines_occurrance.first;
            total_lines_occ.push_back(lines_occurrance.second);
        }

        if (options.is_count)
        {
            cout << total_occ << '\n';
            return 0;
        }

        for (auto files_line : total_lines_occ)
        {
            for(auto line : files_line){
                cout << line << '\n';
            }
        }
    } else if(options.action == "zip") {

    } else if(options.action == "unzip"){

    } else {
        PrintHelp();
        return 1;
    }

    return 0;
}