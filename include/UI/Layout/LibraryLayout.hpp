#pragma once

#include "UI/Layout/ScreenLayout.hpp"

struct LibraryLayout : public ScreenLayout
{
    int ItemWidth = 0;

    int ItemHeight = 20;

    int Columns = 1;

    LibraryLayout() {
        FooterHeight = 0;
    }

    size_t VisibleRows() const
    {
        return ContentHeight /
               ItemHeight;
    }

    size_t VisibleItemsPerPage() const
    {
        return VisibleRows() *
               Columns;
    }
};
