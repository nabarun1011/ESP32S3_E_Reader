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

Page TextRenderer::BuildPage(
    const String &text,
    size_t startLine,
    int availableHeight)
{
    Page page;

    String normalized = text;

    normalized.replace("\r\n", "\n");
    normalized.replace('\r', '\n');

    int start = 0;
    size_t currentLine = 0;

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
                m_display.Width() - 20);

        if (wrapped.empty())
        {
            if (currentLine >= startLine)
            {
                if (page.Lines.size() >= MaxVisibleLines(availableHeight))
                {
                    page.NextPageStartLine = currentLine;
                    return page;
                }

                page.Lines.push_back("");
            }

            currentLine++;
        }
        else
        {
            // Iterate throught wrapped paragraph lines
            for (const auto &line : wrapped)
            {
                if (currentLine >= startLine)
                {
                    if (page.Lines.size() >= MaxVisibleLines(availableHeight))
                    {
                        page.NextPageStartLine = currentLine;
                        return page;
                    }

                    page.Lines.push_back(line);
                }

                currentLine++;
            }
        }

        start = end + 1;
    }

    page.NextPageStartLine = currentLine;
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
            page.Lines[i],
            m_font);
    }
}

std::vector<String> TextRenderer::WrapText(
    const String &text,
    int maxWidth)
{
    std::vector<String> lines;

    String currentLine;

    int start = 0;

    while (start < text.length())
    {
        int end = text.indexOf(' ', start);

        if (end == -1)
        {
            end = text.length();
        }

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
                lines.push_back(currentLine);
            }

            currentLine = word;
        }

        start = end + 1;
    }

    if (!currentLine.isEmpty())
    {
        lines.push_back(currentLine);
    }

    return lines;
}
