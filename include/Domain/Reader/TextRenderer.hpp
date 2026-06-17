#pragma once

#include "Interfaces/IDisplay.hpp"
#include "Domain/Reader/Page.hpp"

class TextRenderer
{
public:
    explicit TextRenderer(
        IDisplay &display);

    size_t MaxVisibleLines() const;

    Page BuildPage(const String &text, size_t startLine);

    void DrawPage(int x, int y, const Page &page);

private:
    std::vector<String> WrapText(
        const String &text,
        int maxWidth);

    IDisplay &m_display;
};