#pragma once

#include "Interfaces/IDisplay.hpp"
#include "Domain/Reader/Page.hpp"
#include "Domain/Reader/WrappedLine.hpp"

class TextRenderer
{
public:
    explicit TextRenderer(
        IDisplay &display);

    size_t MaxVisibleLines(int avaialableHeight) const;

    Page BuildPageFromLines(const std::vector<WrappedLine> &lines, size_t startLine, int availableHeight);

    void DrawPage(int x, int y, const Page &page);

    void SetFont(ReaderFont font);

    std::vector<WrappedLine> WrapDocument(
        const String &text,
        int maxWidth);

private:
    std::vector<WrappedLine> WrapText(
        const String &text,
        size_t paragraphOffset,
        int maxWidth);

    ReaderFont m_font = ReaderFont::Serif;
    IDisplay &m_display;
};