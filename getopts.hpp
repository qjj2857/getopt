// Simple but powerful command-line options handler (C++11).
// by qjj2857

#pragma once
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#define GETOPTS_CONVERT_EQUALS

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

namespace getopts {

    //string converter

#ifdef _WIN32

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
#endif

    template< typename T >
    inline T cvt(const std::string& in_)
    {
        T t;
        if (std::istringstream(in_) >> t)
        {
            return t;
        }
        else
        {
            return  (T)(in_.size() && (in_ != "0") && (in_ != "false"));
        }
    }

    template<>
    inline char cvt(const std::string& in_)
    {
        return in_.size() == 1 ? (char)(in_[0]) : (char)(cvt<int>(in_));
    }
    template<>
    inline signed char cvt(const std::string& in_)
    {
        return in_.size() == 1 ? (signed char)(in_[0]) : (signed char)(cvt<int>(in_));
    }
    template<>
    inline unsigned char cvt(const std::string& in_)
    {
        return in_.size() == 1 ? (unsigned char)(in_[0]) : (unsigned char)(cvt<int>(in_));
    }

    template<>
    inline int cvt(const std::string& in_)
    {
        return std::stoi(in_);
    }

    template<>
    inline unsigned long cvt(const std::string& in_)
    {
        return std::stoul(in_);
    }

    template<>
    inline long long cvt(const std::string& in_)
    {
        return std::stoll(in_);
    }

    template<>
    inline unsigned long long cvt(const std::string& in_)
    {
        return std::stoull(in_);
    }

    template<>
    inline const char* cvt(const std::string& in_)
    {
        return in_.c_str();
    }
    template<>
    inline std::string cvt(const std::string& in_)
    {
        return in_;
    }


    inline std::string GetCmdline()
    {
        std::string cmds;
#ifdef _WIN32
        auto cmdline = GetCommandLineW();// for debug
        std::wstring w_cmds(cmdline);
        cmds = WstringToString(w_cmds);
#else
        std::string arg;
        pid_t pid = getpid();

        char fname[32] = {};
        sprintf(fname, "/proc/%d/cmdline", pid);
        std::ifstream ifs(fname);
        if (ifs.good())
        {
            std::stringstream ss;
            ifs >> ss.rdbuf();
            arg = ss.str();
        }
        for (auto end = arg.size(), i = end - end; i < end; ++i)
        {
            auto st = i;
            while (i < arg.size() && arg[i] != '\0') ++i;
            cmds += (arg.substr(st, i - st));
            cmds += ' ';
        }
        cmds = cmds.substr(0, cmds.size() - 1); //remove last space added by myself
#endif
        return cmds;
    }

    inline std::vector<std::string> ParseCmdLine(const std::string& cmdline_)
    {
        std::vector<std::string> args;
        std::string arg;
#ifndef GETOPTS_CONVERT_EQUALS
        const std::string& cmdline = cmdline_;
#else
        std::string cmdline = cmdline_;
        std::replace(cmdline.begin(), cmdline.end(), '=', ' ');
#endif // GETOPTS_CONVERT_EQUALS

        size_t s_idx = 0;    //start index
        size_t idx = cmdline.find(" ");

        while (idx != cmdline.npos)
        {
            if (idx != s_idx)      //continuous spaces
            {
                arg = cmdline.substr(s_idx, idx - s_idx);
                args.emplace_back(arg);
            }
            s_idx = idx + 1;
            args.emplace_back(" ");
            idx = cmdline.find(" ", s_idx);
        }

        arg = cmdline.substr(s_idx);
        args.emplace_back(arg);

        return args;
    }

    class GetOpt
    {
    public:
        GetOpt(std::string cmdline_) : GetOpt(ParseCmdLine(cmdline_))
        {
        }
        GetOpt() : GetOpt(ParseCmdLine(GetCmdline()))
        {
        }
        GetOpt(const std::vector<std::string>& args_)
        {
            const auto& args = args_;
            auto size = args.size();

            for (int i = 0; i < size; ++i)
            {
                const auto& arg = args[i];

                if (arg[0] == '-')              //find arg key
                {
                    std::string key(arg);
                    m_args_map[key] = "";

                    bool found_1st_none_space = false;
                    int space_num = 0;           //for removing trailing space
                    std::string full_content;
                    for (++i; i < size; ++i)
                    {
                        auto& content = args[i];
                        if (content[0] == '-')  // next arg key found
                        {
                            --i;
                            break;
                        }
                        else if (content == " ")   //continuous content separated by spaces, keep the num of spaces
                        {
                            if (found_1st_none_space)
                            {
                                space_num++;
                            }
                        }
                        else     //drop spaces between key and valid content
                        {
                            if (space_num != 0)
                            {
                                full_content.append(space_num, ' ');
                                space_num = 0;
                            }
                            found_1st_none_space = true;
                            full_content += content;
                        }
                    }

                    if (!full_content.empty())
                    {
                        //deal with input: "c:/ prog file"  , where the '"' is typed user
                        if (full_content.front() == '\"' && full_content.back() == '\"')
                        {
                            full_content = std::string(full_content.begin() + 1, full_content.end() - 1);
                        }
                        m_args_map[key] = full_content;
                    }
                }
            }
        }

        bool HasArg(const std::string& arg_) const
        {
            if (arg_.front() != '-')
            {
                return m_args_map.find('-' + arg_) != m_args_map.end();
            }
            else
            {
                return m_args_map.find(arg_) != m_args_map.end();
            }
        }

        template< typename... Args >
        bool HasArg(const std::string& arg_, Args... args_) const
        {
            if (!HasArg(arg_))
            {
                return HasArg(args_...);
            }
            else
            {
                return true;
            }
        }


        template< typename T >
        //T GetArg(const T& default_val_, const char* arg_)
        T GetArg(const T& default_val_, const std::string& arg_)
        {
            if (HasArg(arg_))
            {
                auto info = GetArgVal(arg_);
                return cvt<T>(info);
            }
            else
            {
                return default_val_;
            }
        }

        template< typename T, typename... Args >
        //T GetArg(const T& default_val_, const char* arg_, Args... args_)
        T GetArg(const T& default_val_, const std::string& arg_, Args... args_)
        {
            T t = GetArg<T>(default_val_, arg_);
            if (t == default_val_)
            {
                return GetArg<T>(default_val_, args_...);
            }
            else
            {
                return t;
            }
        }

        //std::string GetArg(const char* default_val_, const char* arg_)
        std::string GetArg(const std::string& default_val_, const std::string& arg_)
        {
            if (HasArg(arg_))
            {
                return GetArgVal(arg_);
            }
            else
            {
                return default_val_;
            }
        }

        template< typename... Args >
        //std::string  GetArg(const char* default_val_, const char* arg_, Args... args_)
        std::string  GetArg(const std::string& default_val_, const std::string& arg_, Args... args_)
        {
            auto t = GetArg(default_val_, arg_);
            if (t == default_val_)
            {
                return GetArg(default_val_, args_...);
            }
            else
            {
                return t;
            }
        }

    private:
        std::string GetArgVal(const std::string& arg_)
        {
            if (arg_.front() != '-')
            {
                return m_args_map['-' + arg_];
            }
            else
            {
                return m_args_map[arg_];
            }
        };
        std::unordered_map< std::string, std::string > m_args_map;
    };
}

#ifdef GETOPTS_CONVERT_EQUALS
#undef GETOPTS_CONVERT_EQUALS
#endif // GETOPTS_CONVERT_EQUALS
