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

using namespace std;

struct ipmt_options
{
};

void PrintHelp()
{
    cout << "--help: Mostra esta mensagem de ajuda\n";
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        PrintHelp();
        return 1;
    }

    return 0;
}