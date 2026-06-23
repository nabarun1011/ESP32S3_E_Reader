#pragma once

#include <vector>
#include <Domain/Reader/WrappedLine.hpp>

struct Page
{
    std::vector<WrappedLine> Lines;
    // Line index of first line of next page
    // The next wrapped line that wasn't displayed.
    size_t FirstCharacterOffset = 0;

    size_t NextPageStartLine = 0;
};