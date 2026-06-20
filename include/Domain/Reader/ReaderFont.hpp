#pragma once

#include <Arduino.h>

enum class ReaderFont
{
    Serif,
    Sans,
    Mono,

    Count
};

inline const char *ToString(ReaderFont font)
{
    switch (font)
    {
    case ReaderFont::Serif:
        return "Serif";

    case ReaderFont::Sans:
        return "Sans";

    case ReaderFont::Mono:
        return "Mono";
    }

    return "";
}

