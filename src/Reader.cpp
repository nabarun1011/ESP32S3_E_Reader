#include <Reader.h>
#include <ReaderSettings.h>
#include <Pins.h>
#include<UIRenderer.h>

int GetLineHeight()
{
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds("Ag", 0, 0, &x1, &y1, &w, &h);
    return h + lineSpacing;
}

void Reader::OpenBook(const BookInfo &bookInfo)
{
    currentBook.bookInfo = bookInfo;
    currentBook.currentPage = 0;

    File file = currentBook.bookInfo.storage->open(bookInfo.path, "r");

    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    String text = file.readString() + " ";
    file.close();

    // Parse the text file into paragraphs
    if (bookInfo.path.endsWith(".txt"))
    {
        ParseBook(text);
        BuildPages();
    }
}

void Reader::ParseBook(String text)
{
    currentBook.paragraphs.clear();

    String currentParagraph = "";

    for (int i = 0; i < text.length(); i++)
    {
        char c = text[i];

        // If character is a newline, add the current paragraph to the list
        if (c == '\n')
        {

            currentBook.paragraphs.push_back(currentParagraph);
            currentParagraph = "";
        }
        // Otherwise, add the character to the current paragraph
        else
        {
            currentParagraph += c;
        }
    }
}

void Reader::BuildPages()
{
    currentBook.PageStarts.clear();

    // Add the first page start with 0 and 0 as the paragraph and character indices
    currentBook.PageStarts.push_back(PageStart{0, 0});

    int y = topMargin;

    int contentWidth = display.width() - leftMargin - rightMargin;

    int MaxY = display.height() - bottomMargin;

    int lineHeight = GetLineHeight();

    for (int paragraphIndex = 0; paragraphIndex < currentBook.paragraphs.size(); paragraphIndex++)
    {
        String paragraph = currentBook.paragraphs[paragraphIndex];

        String currentLine = "";

        String currentWord = "";

        for (int characterIndex = 0; characterIndex < paragraph.length(); characterIndex++)
        {

            char c = paragraph[characterIndex];

            // Add the current word to the current line if it's a newline
            if (c == '\n')
            {
                y += lineHeight;
                if (y + lineHeight > MaxY)
                {
                    PageStart pageStart;
                    pageStart.paragraphIndex = paragraphIndex;
                    pageStart.characterIndex = characterIndex - currentWord.length();
                    currentBook.PageStarts.push_back(pageStart);
                    y = topMargin;
                }
                currentLine = "";
                currentWord = "";
            }
            // Add the current word to the current line if it's a space or the end of the paragraph
            else if (c == ' ' || characterIndex == paragraph.length() - 1)
            {
                // If the current word is the last word in the paragraph, add it to the current word
                if (characterIndex == paragraph.length() - 1 || c == ' ')
                {
                    currentWord += c;
                }

                String testLine = currentLine + currentWord;

                // x and y position of textline
                int16_t tbx, tby;
                // width and height of text line
                uint16_t tbw, tbh;

                display.getTextBounds(testLine, 0, 0, &tbx, &tby, &tbw, &tbh);

                // If the current word doesn't fit on the current line, start a new line with current as first word of current line
                if (tbw > contentWidth)
                {
                    y += lineHeight;
                    if (y + lineHeight > MaxY)
                    {
                        PageStart pageStart;
                        pageStart.paragraphIndex = paragraphIndex;
                        pageStart.characterIndex = characterIndex - currentWord.length();
                        currentBook.PageStarts.push_back(pageStart);
                        y = topMargin;
                    }
                    currentLine = currentWord;
                }
                else
                {
                    currentLine += currentWord;
                }

                // Reset the current word
                currentWord = "";
            }
            // Otherwise, add the character to the current word
            else
            {
                currentWord += c;
            }
        }
        y += lineSpacing;
    }

    currentBook.totalPages = currentBook.PageStarts.size();
}

void Reader::BuildCurrentPageLines()
{
    currentBook.wrappedCurrentPageLines.clear();

    int firstParagraphIndex = currentBook.PageStarts[currentBook.currentPage].paragraphIndex;
    int lastParagraphIndex = currentBook.PageStarts[currentBook.currentPage + 1].paragraphIndex;

    int firstCharacterIndex = currentBook.PageStarts[currentBook.currentPage].characterIndex;

    int y = topMargin;

    int contentWidth = display.width() - leftMargin - rightMargin;

    int MaxY = display.height() - bottomMargin;

    int lineHeight = GetLineHeight();

    for (int paragraphIndex = firstParagraphIndex; paragraphIndex < lastParagraphIndex; paragraphIndex++)
    {
        String paragraph = currentBook.paragraphs[paragraphIndex];

        String currentLine = "";

        String currentWord = "";

        for (int characterIndex = 0; characterIndex < paragraph.length(); characterIndex++)
        {

            char c = paragraph[characterIndex];

            if (c == '\n')
            {
                y += lineHeight;
                currentBook.wrappedCurrentPageLines.push_back(currentLine);
                if (y + lineHeight > MaxY)
                {
                    break;
                }
                currentLine = "";
                currentWord = "";
            }
            // Add the current word to the current line if it's a space or the end of the paragraph
            else if (c == ' ' || characterIndex == paragraph.length() - 1)
            {
                // If the current word is the last word in the paragraph, add it to the current word
                if (characterIndex == paragraph.length() - 1 || c == ' ')
                {
                    currentWord += c;
                }

                String testLine = currentLine + currentWord;

                // x and y position of textline
                int16_t tbx, tby;
                // width and height of text line
                uint16_t tbw, tbh;

                display.getTextBounds(testLine, 0, 0, &tbx, &tby, &tbw, &tbh);

                // If the current word doesn't fit on the current line, start a new line with current as first word of current line
                if (tbw > contentWidth)
                {
                    y += lineHeight;
                    currentBook.wrappedCurrentPageLines.push_back(currentLine);
                    if (y + lineHeight > MaxY)
                    {
                        break;
                    }
                    currentLine = currentWord;
                }
                else
                {
                    currentLine += currentWord;
                }

                // Reset the current word
                currentWord = "";
            }
            // Otherwise, add the character to the current word
            else
            {
                currentWord += c;
            }
        }
        y += lineSpacing;
    }
}

ReaderInputResults Reader::HandleInput(InputAction action)
{
    switch (action)
    {
    case Action_DOWN:
        if (currentBook.currentPage < currentBook.totalPages - 1)
        {
            currentBook.currentPage++;
            BuildCurrentPageLines();
        }
        break;
    case Action_UP:
        if (currentBook.currentPage > 0)
        {
            currentBook.currentPage -= 1;
            BuildCurrentPageLines();
        }
        break;
    case Action_BACK:
        return InputResult_Back;
    default:
        break;
    }

    return InputResult_None;
}

void Reader::Render()
{
    display.fillScreen(GxEPD_WHITE);

    int y = topMargin;

    int lineHeight = GetLineHeight();

    for (int i = 0; i < currentBook.wrappedCurrentPageLines.size(); i++)
    {
        display.setCursor(leftMargin, y);
        y += lineHeight;
    }

    // // Draw footer with page number
    // display.setFont(&FreeSansBold9pt7b);
    // String footerText = String(book.currentPage) + " / " + String(book.totalPages);
    // display.setCursor(leftMargin, display.height() - footerHeight / 2);
    // display.println(footerText);
    // display.setFont(GetCurrentFont());
}