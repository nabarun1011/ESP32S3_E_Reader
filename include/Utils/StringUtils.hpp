#pragma once

#include <string>
#include <string_view>

class StringUtils
{
public:
    static std::string ToLower(std::string str);

    static bool StartsWith(std::string_view str,
                           std::string_view prefix);

    static bool EndsWith(std::string_view str,
                         std::string_view suffix);

    static bool Contains(std::string_view str,
                         std::string_view text);

    static bool EqualsIgnoreCase(std::string_view lhs,
                                 std::string_view rhs);

    static void Replace(std::string &str,
                        char oldChar,
                        char newChar);
    static void Replace(std::string &str,
                        std::string_view oldStr,
                        std::string_view newStr);
};