#include "Domain/Reader/TextRenderer.hpp"
#include "Utils/StringUtils.hpp"

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
    const std::string &text,
    int maxWidth)
{
    std::vector<WrappedLine> lines;

    std::string normalized = text;

    StringUtils::Replace(normalized, "\r\n", "\n");
    StringUtils::Replace(normalized, '\r', '\n');

    int start = 0;

    while (start < normalized.length())
    {
        size_t end =
            normalized.find(
                '\n',
                start);

        if (end == std::string::npos)
        {
            end = normalized.length();
        }

        std::string paragraph =
            normalized.substr(
                start,
                end - start);

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
    const std::string &text,
    size_t paragraphOffset,
    int maxWidth)
{
    std::vector<WrappedLine> lines;

    size_t currentLineOffset = paragraphOffset;
    size_t currentWordOffset = paragraphOffset;

    std::string currentLine;

    int start = 0;

    while (start < text.length())
    {
        size_t end = text.find(' ', start);

        if (end == std::string::npos)
        {
            end = text.length();
        }

        currentWordOffset = start + paragraphOffset;
        std::string word =
            text.substr(start, end - start);

        std::string candidate =
            currentLine.empty()
                ? word
                : currentLine + " " + word;

        if (m_display.GetTextWidth(candidate, m_font) <= maxWidth)
        {
            currentLine = candidate;
        }
        else
        {
            if (!currentLine.empty())
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

    if (!currentLine.empty())
    {
        WrappedLine line;

        line.Text = currentLine;

        line.StartOffset = currentLineOffset;

        lines.push_back(line);
    }

    return lines;
}
