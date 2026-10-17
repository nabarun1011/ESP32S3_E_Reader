#include "UI/Screens/ReaderScreen.hpp"

static const char *TAG = "ReaderScreen";

ReaderScreen::ReaderScreen(
    IStorage &storage,
    TextDocument &document,
    IDisplay &display,
    DeviceSettings &deviceSettings,
    IBookSettingsRepository &bookSettingsRepository)
    : BaseScreen(deviceSettings, display),
      m_storage(storage),
      m_document(document),
      m_renderer(display),
      m_bookSettingsRepository(bookSettingsRepository),
      m_overlay(
          display,
          deviceSettings,
          m_bookSettings,
          m_readerSettings,
          [this]()
          {
              UpdateLayout();
          },

          [this](size_t page)
          {
              GoToPage(page);
          },
          m_pageStarts)
{
}

void ReaderScreen::Enter()
{
    Serial.printf("%s : Enter\n", TAG);
    auto file = m_storage.Open(m_currentBook.path, FileMode::Read);

    if (!file)
    {
        return;
    }

    m_document.Open(std::move(file));

    m_text = m_document.ReadAll();

    ApplyReaderSettings();

    GoToPage(m_readerSettings.CurrentPage);

    Refresh(true);

}

void ReaderScreen::ApplyReaderSettings()
{

    if (m_readerSettings.CurrentPage < m_pageStarts.size())
    {
        m_bookSettings.LastCharacterOffset = m_pageStarts[m_readerSettings.CurrentPage].characterOffset;
    }
    m_display.SetRotation(m_deviceSettings.Orientation);
    m_renderer.SetFont(m_bookSettings.Font);
    m_display.SetTextSize(m_bookSettings.FontSize);

    UpdateLayout();

    m_wrappedLines = m_renderer.WrapDocument(m_text, m_display.Width() - 20);
    RebuildPageIndex();

    m_readerSettings.CurrentPage = FindPageForOffset(m_bookSettings.LastCharacterOffset);
    if (m_readerSettings.CurrentPage > m_pageStarts.size() - 1)
    {
        m_readerSettings.CurrentPage = m_pageStarts.size() - 1;
    }

}

void ReaderScreen::Exit()
{
    if (m_readerSettings.CurrentPage < m_pageStarts.size())
    {
        m_bookSettings.LastCharacterOffset = m_pageStarts[m_readerSettings.CurrentPage].characterOffset;
    }
    m_bookSettingsRepository.Save(
        m_currentBook.path,
        m_bookSettings);
}

void ReaderScreen ::SetBook(const BookInfo &book)
{
    m_currentBook = book;

    m_bookSettings =
        BookSettings();
    if (!m_bookSettingsRepository.Load(book.path, m_bookSettings))
    {
        Serial.printf("Failed to load book settings for book: %s\n", book.path.c_str());
    }
}

ScreenCommand ReaderScreen::HandleButton(Button button)
{
    bool needRedraw = true;
    if (m_overlay.IsVisible())
    {
        m_overlay.HandleButton(button);
    }
    else
    {
        switch (button)
        {
        case Button::Up:
            PreviousPage();
            break;
        case Button::Down:
            NextPage();
            break;
        case Button::Left:
            PreviousPage();

            break;
        case Button::Right:
            NextPage();

            break;
        case Button::Select:
            Serial.printf("%s : Select\n", TAG);
            m_overlay.Show();
            break;
        case Button::Back:
            return {
                ScreenCommandType::OpenLibrary};
            break;
        default:
            break;
        }
    }
    if (needRedraw)
    {
        Refresh();
    }

    return {};
}

void ReaderScreen::RebuildPageIndex()
{
    m_pageStarts.clear();

    size_t startLine = 0;

    while (true)
    {
        Serial.printf("ContentHeight = %d\n", m_layout.ContentHeight);
        Serial.printf("StartLine = %u\n", (unsigned)startLine);
        Page page = m_renderer.BuildPageFromLines(m_wrappedLines, startLine, m_layout.ContentHeight);
        Serial.printf("Page lines = %u\n", (unsigned)page.Lines.size());
        Serial.printf("Next start = %u\n", (unsigned)page.NextPageStartLine);

        if (page.Lines.empty())
        {
            break;
        }

        PageIndexedEntry pageInfo = {startLine, page.FirstCharacterOffset};

        m_pageStarts.push_back(pageInfo);

        if (page.NextPageStartLine <= startLine)
        {
            break;
        }

        if (page.NextPageStartLine >=
            m_wrappedLines.size())
        {
            break;
        }

        startLine = page.NextPageStartLine;
    }

    for (size_t i = 0;
         i < m_pageStarts.size();
         ++i)
    {
        Serial.printf(
            "Page %u startLine=%u offset=%u\n",
            (unsigned)i,
            (unsigned)m_pageStarts[i].startLine,
            (unsigned)m_pageStarts[i].characterOffset);
    }

    Serial.printf("PageStarts after rebuild = %u\n", m_pageStarts.size());
}

size_t ReaderScreen::FindPageForOffset(size_t offset) const
{
    size_t page = 0;

    for (size_t i = 0;
         i < m_pageStarts.size();
         ++i)
    {
        if (m_pageStarts[i].characterOffset >
            offset)
        {
            break;
        }

        page = i;
    }

    return page;
}

void ReaderScreen::Draw()
{

    // m_display.SetTextSize(m_bookSettings.FontSize);
    // Serial.printf("%s : Set text size\n", TAG);
    Page page = m_renderer.BuildPageFromLines(m_wrappedLines, m_pageStarts[m_readerSettings.CurrentPage].startLine, m_layout.ContentHeight);

    DrawHeader(m_currentBook.title);

    DrawFooter();

    m_renderer.DrawPage(
        10, m_layout.ContentY,
        page);

    if (m_overlay.IsVisible())
    {
        m_overlay.Draw();
    }

}

void ReaderScreen::DrawFooter()
{
    std::string footer;
    footer = "Page " + std::to_string(m_readerSettings.CurrentPage) + "/" + std::to_string(m_pageStarts.size() - 1);

    m_display.DrawText(
        10,
        m_display.Height() - 10,
        footer, m_deviceSettings.UIFont);
}

void ReaderScreen::NextPage()
{
    if (m_readerSettings.CurrentPage < m_pageStarts.size() - 1)
    {
        m_readerSettings.CurrentPage++;
    }
}

void ReaderScreen::PreviousPage()
{
    if (m_readerSettings.CurrentPage > 0)
    {
        m_readerSettings.CurrentPage--;
    }
}

void ReaderScreen::GoToPage(size_t page)
{
    if (page < m_pageStarts.size())
    {
        m_readerSettings.CurrentPage = page;
    }
}