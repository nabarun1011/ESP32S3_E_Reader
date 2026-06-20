#pragma once

#include "Interfaces/IDisplay.hpp"
#include "Domain/Reader/Page.hpp"

class TextRenderer
{
public:
    explicit TextRenderer(
        IDisplay &display);

    size_t MaxVisibleLines(int avaialableHeight) const;

    Page BuildPage(const String &text, size_t startLine, int avaialableHeight);

    void DrawPage(int x, int y, const Page &page);

    void SetFont(ReaderFont font);

private:
    std::vector<String> WrapText(
        const String &text,
        int maxWidth);

    ReaderFont m_font = ReaderFont::Serif;
    IDisplay &m_display;
};