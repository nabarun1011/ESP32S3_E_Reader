#pragma once

#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSerifBold12pt7b.h>

enum FontFamily
{
    Font_Mono,
    Font_Sans,
    Font_Serif,

    Font_Count
};

const String fontFamilyNames[3] = {
    "Mono",
    "Sans",
    "Serif"};

enum FontWeight
{
    Font_Regular,
    Font_Bold
};
const GFXfont *GetCurrentFont(int size = -1);


