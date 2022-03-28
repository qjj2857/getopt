#include "GetOpt.hpp"
#include <iostream>

using namespace std;
using namespace getopt;

int main(void)
{
    GetOpt opts;

    auto info = GetCmdline();
    auto args = ParseCmdLine(info);

    auto num = opts.GetArg(1, "-n", "--num", "--n", "-num");
    auto input = opts.GetArg("", "-i", "--input", "--i", "-input");
    auto output = opts.GetArg("t:/", "--zh");
    bool pause = opts.HasArg("-p");
    bool pause1 = opts.HasArg("--p");

    cout << info << endl;
}