#include "Utils/StringUtils.hpp"

#include <algorithm>
#include <cctype>

std::string StringUtils::ToLower(std::string str)
{
    std::transform(str.begin(),
                   str.end(),
                   str.begin(),
                   [](unsigned char c)
                   {
                       return std::tolower(c);
                   });

    return str;
}

bool StringUtils::StartsWith(std::string_view str,
                             std::string_view prefix)
{
    return str.size() >= prefix.size() &&
           str.compare(0, prefix.size(), prefix) == 0;
}

bool StringUtils::EndsWith(std::string_view str,
                           std::string_view suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(),
                       suffix.size(),
                       suffix) == 0;
}

bool StringUtils::Contains(std::string_view str,
                           std::string_view text)
{
    return str.find(text) != std::string_view::npos;
}

bool StringUtils::EqualsIgnoreCase(std::string_view lhs,
                                   std::string_view rhs)
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }

    for (size_t i = 0; i < lhs.size(); ++i)
    {
        if (std::tolower(static_cast<unsigned char>(lhs[i])) !=
            std::tolower(static_cast<unsigned char>(rhs[i])))
        {
            return false;
        }
    }

    return true;
}

void StringUtils::Replace(std::string &str,
                          char oldChar,
                          char newChar)
{
    std::replace(str.begin(),
                 str.end(),
                 oldChar,
                 newChar);
}

void StringUtils::Replace(
    std::string &text,
    std::string_view from,
    std::string_view to)
{
    size_t pos = 0;

    while ((pos = text.find(from, pos)) != std::string::npos)
    {
        text.replace(pos, from.length(), to);
        pos += to.length();
    }
}