#pragma once

#include "Interfaces/IDisplay.hpp"

class TextRenderer
{
public:
    explicit TextRenderer(
        IDisplay &display);

    void DrawTextBlock(
        int x,
        int y,
        const String &text);

    size_t MaxVisibleLines() const;

private:
    std::vector<String> WrapText(
        const String &text,
        int maxWidth);

    IDisplay &m_display;
};