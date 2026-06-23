#include "Domain/Reader/TextRenderer.hpp"

TextRenderer::TextRenderer(
    IDisplay &display)
    : m_display(display)
{
}

void TextRenderer::SetFont(ReaderFont font)
{
    m_font = font;
}

size_t TextRenderer::MaxVisibleLines(int availableHeight) const
{
    return availableHeight /
           m_display.LineHeight(m_font);
}

std::vector<WrappedLine>
TextRenderer::WrapDocument(
    const String &text,
    int maxWidth)
{
    std::vector<WrappedLine> lines;

    String normalized = text;

    normalized.replace("\r\n", "\n");
    normalized.replace('\r', '\n');

    int start = 0;

    while (start < normalized.length())
    {
        int end =
            normalized.indexOf(
                '\n',
                start);

        if (end == -1)
        {
            end = normalized.length();
        }

        String paragraph =
            normalized.substring(
                start,
                end);

        auto wrapped =
            WrapText(
                paragraph,
                start,
                maxWidth);

        lines.insert(
            lines.end(),
            wrapped.begin(),
            wrapped.end());

        if (wrapped.empty())
        {
            lines.push_back(
                {"",
                 static_cast<size_t>(start)});
        }

        start = end + 1;
    }

    return lines;
}

Page TextRenderer::BuildPageFromLines(
    const std::vector<WrappedLine> &lines,
    size_t startLine,
    int availableHeight)
{
    Page page;

    const size_t maxLines =
        MaxVisibleLines(
            availableHeight);

    for (size_t i = startLine;
         i < lines.size();
         ++i)
    {
        if (page.Lines.empty())
        {
            page.FirstCharacterOffset =
                lines[i].StartOffset;
        }

        if (page.Lines.size() >= maxLines)
        {
            page.NextPageStartLine = i;
            return page;
        }

        page.Lines.push_back(
            lines[i]);
    }

    page.NextPageStartLine =
        lines.size();

    return page;
}



void TextRenderer::DrawPage(
    int x,
    int y,
    const Page &page)
{
    for (size_t i = 0;
         i < page.Lines.size();
         i++)
    {
        m_display.DrawText(
            x,
            y +
                i *
                    m_display.LineHeight(m_font),
            page.Lines[i].Text,
            m_font);
    }
}

std::vector<WrappedLine> TextRenderer::WrapText(
    const String &text,
    size_t paragraphOffset,
    int maxWidth)
{
    std::vector<WrappedLine> lines;

    size_t currentLineOffset = paragraphOffset;
    size_t currentWordOffset = paragraphOffset;

    String currentLine;

    int start = 0;

    while (start < text.length())
    {
        int end = text.indexOf(' ', start);

        if (end == -1)
        {
            end = text.length();
        }

        currentWordOffset = start + paragraphOffset;
        String word =
            text.substring(start, end);

        String candidate =
            currentLine.isEmpty()
                ? word
                : currentLine + " " + word;

        if (m_display.MeasureTextWidth(candidate, m_font) <= maxWidth)
        {
            currentLine = candidate;
        }
        else
        {
            if (!currentLine.isEmpty())
            {
                WrappedLine line;

                line.Text = currentLine;

                line.StartOffset = currentLineOffset;

                lines.push_back(line);
            }

            currentLine = word;
            currentLineOffset = currentWordOffset;
        }

        start = end + 1;
    }

    if (!currentLine.isEmpty())
    {
        WrappedLine line;

        line.Text = currentLine;

        line.StartOffset = currentLineOffset;

        lines.push_back(line);
    }

    return lines;
}
