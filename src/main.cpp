#include <getopt.h>
#include <glob.h>
#include <bits/stdc++.h>
#include <sarr_index.h>
#include <sarr_search.h>
#include "zip.h"
#include "unzip.h"

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

    if(options.action == "index") {
        for(auto file_name : options.files)
        {
            string line, aux_file_name = file_name;
            
            int idx = aux_file_name.find_last_of(".");
            aux_file_name = aux_file_name.replace(idx, file_name.size() - idx, ".idx");

            ifstream file(file_name);
            ofstream indexed_file(aux_file_name, ios::binary | ios::out);
            vector<vector<int>> P;
            vector<int> llcp, rlcp, sarr;

            while(getline(file, line)){
                int n = (int)line.size();
                if(n < 1) continue; // Jump empty lines
                int log2N = (int)ceil(log2(n));
                P.resize(max(log2N+1, 2));
                llcp.resize(n); rlcp.resize(n);
                fill(llcp.begin(), llcp.end(), -1); fill(rlcp.begin(), rlcp.end(), -1);
                sarr.resize(n);

                build_P(P, line);
                build_sarr(sarr, P.back());
                build_LR_lcp(line, P, sarr, llcp, rlcp, 0, (int)sarr.size()-1);

                indexed_file.write(reinterpret_cast<const char *>(&n), sizeof(int));
                write_vector_in_file(P[0], indexed_file);
                write_vector_in_file(sarr, indexed_file);
                write_vector_in_file(llcp, indexed_file);
                write_vector_in_file(rlcp, indexed_file);
            }

            file.close(); indexed_file.close();
        }

    } else if(options.action == "search"){
        auto patterns = get_patterns_from_options(options);
        long long total_occ = 0;
        vector<string> total_lines_occ;

        for(auto file_name : options.files)
        {
            ifstream file(file_name);
            int n;
            vector<int> p0, llcp, rlcp, sarr;

            while(file.read(reinterpret_cast<char *>(&n), sizeof(int)) && n){
                p0.resize(n); llcp.resize(n); rlcp.resize(n); sarr.resize(n);
                read_vector_from_file(p0, file);
                read_vector_from_file(sarr, file);
                read_vector_from_file(llcp, file);
                read_vector_from_file(rlcp, file);
                bool included_line = false;

                string txt(p0.begin(), p0.end());
                for(string pat : patterns){
                    auto occ = search_sarr(txt, pat, sarr, llcp, rlcp);
                    if(!occ.empty() && !included_line){
                        total_lines_occ.push_back(txt);
                        included_line = true;
                    }
                    total_occ += (long long)occ.size();
                }
            }

            file.close();
        }

        if (options.is_count)
        {
            cout << total_occ << '\n';
            return 0;
        }

        for (auto lines : total_lines_occ)
            cout << lines << '\n';

    } else if(options.action == "zip") {
        auto in_file_name = string(argv[2]);

        ifstream input_file(in_file_name);
        string text;
        std::unordered_map<char, int> occurrences;
        while (true)
        {
            char next;
            input_file.read(&next, sizeof(char));
            if (input_file.eof()) break;
            text += next;
            occurrences[next]++;
        }

        auto root = build_huffman_tree(occurrences);
        auto encoding_dictionary = get_encoding_dictionary(root);
        auto encoded_text = encode(encoding_dictionary, text);

        ofstream out_file;
        auto out_file_name = in_file_name + ".myz";
        out_file.open(out_file_name, ios::binary | ios::out);
        write_encoded_file(out_file, encoding_dictionary, encoded_text);
    }
    else if (options.action == "unzip")
    {
        auto in_file_name = string(argv[2]);

        ifstream in_file;
        in_file.open(in_file_name, ios::binary | ios::in);

        auto decoding_dictionary = get_decoding_dictionary_from_file(in_file);
        auto encoded_text = get_encoded_text_from_file(in_file);

        auto decoded = decode(decoding_dictionary, encoded_text);

        auto out_file_name = in_file_name.substr(0, in_file_name.size() - 4) + ".original";
        ofstream out_file(out_file_name);
        out_file << decoded;
        out_file.close();
    }
    else
    {
        PrintHelp();
        return 1;
    }

    return 0;
}