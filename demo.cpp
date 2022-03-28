#include "GetOpt.hpp"
#include <iostream>

using namespace std;
using namespace getopt;

int main(void)
{

    GetOpt opts;

    auto num = opts.GetArg(1, "-n", "--num", "--n", "-num");
    auto input = opts.GetArg("", "-i", "--input", "--i", "-input");
    auto output = opts.GetArg("t:/", "--zh");
    bool pause = opts.HasArg("--p", "-p1");

    GetOpt opts_user("--n 456 -i  t:/a b.txt --zh a:/中文路径 带  空格  -p");
    auto u_num = opts_user.GetArg(1, "-n", "--num", "--n", "-num");
    auto u_input = opts_user.GetArg("", "-i", "--input", "--i", "-input");
    auto u_output = opts_user.GetArg("t:/", "--zh");
    bool u_pause = opts_user.HasArg("--p", "-p1");

    auto info = GetCmdline();
    auto args = ParseCmdLine(info);
    cout << info << endl;
}