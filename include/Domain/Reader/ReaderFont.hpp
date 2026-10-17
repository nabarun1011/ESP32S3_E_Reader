#pragma once

enum class ReaderFont
{
    Serif,
    Sans,
    Mono,

    Count
};

inline const std::string ToString(ReaderFont font)
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

