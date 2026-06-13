#pragma once

#include<FontManager.h>

struct ReaderSettings
{
    FontFamily fontFamily;
    int fontSizeIndex;
};

constexpr int leftMargin = 10;
constexpr int rightMargin = 10;
constexpr int topMargin = 5;
constexpr int bottomMargin = 0;
constexpr int lineSpacing = 20;