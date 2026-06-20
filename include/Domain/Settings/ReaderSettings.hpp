#pragma once

#include <cstddef>
#include "Domain/Reader/ReaderFont.hpp"

struct ReaderSettings
{
    size_t CurrentPage = 0;

    size_t GoToPage = 0;

    size_t SelectedMenuItem = 0;
};