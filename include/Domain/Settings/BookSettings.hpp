#pragma once

#include <Arduino.h>
#include "Domain/Reader/ReaderFont.hpp"

struct BookSettings
{
    int FontSize = 1;

    ReaderFont Font = ReaderFont::Serif;

    size_t LastCharacterOffset = 0;
};