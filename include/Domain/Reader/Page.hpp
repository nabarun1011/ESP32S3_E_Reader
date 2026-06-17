#pragma once

#include <vector>

struct Page
{
    std::vector<String> Lines;
    // Line index of first line of next page
    // The next wrapped line that wasn't displayed.
    size_t NextPageStartLine = 0;
};