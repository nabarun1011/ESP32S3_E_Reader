#include "Domain/Reader/TextRenderer.hpp"

TextRenderer::TextRenderer(
    IDisplay &display)
    : m_display(display)
{
}

size_t TextRenderer::MaxVisibleLines() const
{
    Serial.printf(
        "Lines/Page = %u\n",
        m_renderer.MaxVisibleLines());
    return m_display.Height() /
           m_display.LineHeight();
}

void TextRenderer::DrawTextBlock(
    int x,
    int y,
    const String &text)
{
    const int lineHeight =
        m_display.LineHeight();

    String normalized = text;

    normalized.replace("\r\n", "\n");
    normalized.replace('\r', '\n');

    int currentY = y;

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
                m_display.Width() - x - 10);

        if (wrapped.empty())
        {
            currentY += lineHeight;
        }
        else
        {
            for (const auto &line : wrapped)
            {
                m_display.DrawText(
                    x,
                    currentY,
                    line);

                currentY += lineHeight;
            }
        }

        start = end + 1;
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

        if (m_display.MeasureTextWidth(candidate) <= maxWidth)
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
