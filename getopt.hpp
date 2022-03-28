// Simple but powerful command-line options handler (C++11).
// by qjj2857
// GPL-3.0 Licensed

#pragma once
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>

#ifdef _WIN32
#include <io.h>
#include <WinSock2.h>
#include <shellapi.h>
#pragma comment(lib, "Shell32.lib")
#else
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif

namespace getopt {

    inline std::wstring StringToWstring(const std::string& str)
    {
        LPCSTR pszSrc = str.c_str();
        int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
        if (nLen == 0)
            return std::wstring(L"");

        wchar_t* pwszDst = new wchar_t[nLen];
        if (!pwszDst)
            return std::wstring(L"");

        MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
        std::wstring wstr(pwszDst);
        delete[] pwszDst;
        pwszDst = NULL;

        return wstr;
    }

    inline std::string WstringToString(const std::wstring& wstr)
    {
        LPCWSTR pwszSrc = wstr.c_str();
        int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
        if (nLen == 0)
            return std::string("");

        char* pszDst = new char[nLen];
        if (!pszDst)
            return std::string("");

        WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
        std::string str(pszDst);
        delete[] pszDst;
        pszDst = NULL;

        return str;
    }

    inline std::string GetCmdline()
    {
        std::string cmds;
#ifdef _WIN32
        auto cmdline = GetCommandLineW();// for debug
        std::wstring w_cmds(cmdline);
        cmds = WstringToString(w_cmds);
#else

#endif
        return cmds;
    }

}

