#ifndef TOOLS_STRING_H
#define TOOLS_STRING_H

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <string>


namespace tools
{
    //checks if a string {str} is prefixed with {prefix}
    static inline bool beginsWith(std::string str, std::string prefix)
    {
        return std::mismatch(prefix.begin(), prefix.end(), str.begin()).first == prefix.end();
    }

    // trim from start
    static inline std::string &ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    // trim from end
    static inline std::string &rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    // trim from both ends
    static inline std::string &trim(std::string &s)
    {
        return ltrim(rtrim(s));
    }

    static inline std::vector<std::string> split(std::string str, std::string delimiter)
    {
        std::vector<std::string> tokens;

        std::size_t pos = 0;
        std::string token;
        while ((pos = str.find(delimiter)) != std::string::npos)
        {
            token = str.substr(0, pos);
            if (!token.empty())
                tokens.push_back(trim(token));
            str.erase(0, pos + delimiter.length());
        }
        if (!str.empty())
            tokens.push_back(trim(str));

        return tokens;
    }
}

#endif
