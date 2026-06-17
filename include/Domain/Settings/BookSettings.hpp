#pragma once

#include <Arduino.h>

struct BookSettings
{
    int FontSize = 16;

    String Font = "Default";

    size_t LastCharacterOffset = 0;
};