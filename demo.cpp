#include "getopt.hpp"
#include <iostream>

using namespace std;
using namespace getopt;

int main(int argc, const char** argv)
{
    auto info = GetCmdline();

    cout << info << endl;
}