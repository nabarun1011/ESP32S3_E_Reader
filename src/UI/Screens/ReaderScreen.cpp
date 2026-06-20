#include "UI/Screens/ReaderScreen.hpp"

ReaderScreen::ReaderScreen(
    TextDocument &document,
    IDisplay &display,
    DeviceSettings &deviceSettings,
    IBookSettingsRepository &bookSettingsRepository)
    : BaseScreen(deviceSettings, display),
      m_document(document),
      m_renderer(display),
      m_bookSettingsRepository(bookSettingsRepository)
{
}

void ReaderScreen::Enter()
{
    char buffer[4096];

    size_t read =
        m_document.Read(
            buffer,
            sizeof(buffer) - 1);

    buffer[read] = '\0';

    m_text = buffer;

    UpdateLayout();

    Serial.printf(
        "Total Pages = %u\n",
        m_pageStarts.size());

    for (size_t i = 0; i < m_pageStarts.size(); i++)
    {
        Serial.printf(
            "Page %u -> StartLine %u\n",
            i + 1,
            m_pageStarts[i]);
    }
}

void ReaderScreen::UpdateLayout()
{
    ApplyReaderSettings();
    // Set footer height based on UI font of page number
    m_layout.FooterHeight = m_display.LineHeight(m_deviceSettings.UIFont) + 4;
    m_layout.TextAreaHeight = m_display.Height() - m_layout.FooterHeight;
    m_layout.FooterY = m_display.Height() - m_layout.FooterHeight;
    RebuildPageIndex();
}

void ReaderScreen::ApplyReaderSettings()
{
    m_readerSettings.GoToPage = m_readerSettings.CurrentPage;
    m_display.SetRotation(m_deviceSettings.Orientation);
    m_renderer.SetFont(m_bookSettings.Font);
    m_display.SetTextSize(m_bookSettings.FontSize);
}

void ReaderScreen::Exit()
{
    m_bookSettingsRepository.Save(
        m_currentBook.path,
        m_bookSettings);
}

void ReaderScreen ::OpenBook(const BookInfo &book)
{
    m_currentBook = book;

    m_bookSettings =
        BookSettings();

    m_bookSettingsRepository.Load(
        book.path,
        m_bookSettings);
    Enter();
}

void ReaderScreen::HandleButton(Button button)
{
    bool needRedraw = true;
    switch (m_state)
    {
    case State::Reading:
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
            ToggleOverlayMenu();
            break;
        case Button::Back:
            // Return to library
            break;
        default:
            break;
        }
        break;
    case State::Menu:
        switch (button)
        {
        case Button::Up:
            MenuUp();

            break;
        case Button::Down:
            MenuDown();

            break;
        case Button::Left:
            MenuLeft();

            break;
        case Button::Right:
            MenuRight();

            break;
        case Button::Select:
            MenuSelect();
            break;
        case Button::Back:
            ToggleOverlayMenu();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    if (needRedraw)
    {
        Refresh();
    }
}

void ReaderScreen::RebuildPageIndex()
{

    m_pageStarts.clear();

    m_pageStarts.push_back(0);

    size_t startLine = 0;

    while (true)
    {
        Page page =
            m_renderer.BuildPage(
                m_text,
                startLine,
                m_layout.TextAreaHeight);

        if (page.NextPageStartLine <= startLine)
        {
            break;
        }

        startLine =
            page.NextPageStartLine;

        m_pageStarts.push_back(
            startLine);
    }
}

void ReaderScreen::ToggleOverlayMenu()
{
    if (m_state == State::Reading)
    {
        m_state = State::Menu;
    }
    else
    {
        m_state = State::Reading;
    }
    Serial.printf("m_state toggled to= %s\n", m_state == State::Menu ? "Menu" : "Reading");
}

void ReaderScreen::Refresh()
{
    m_display.SetTextSize(m_bookSettings.FontSize);

    Page page =
        m_renderer.BuildPage(
            m_text,
            m_pageStarts[m_readerSettings.CurrentPage],
            m_layout.TextAreaHeight);

    m_display.Render(
        [this, page]()
        {
            String footer =
                "Page " +
                String(m_readerSettings.CurrentPage) +
                " / " +
                String(m_pageStarts.size() - 1);

            m_display.DrawText(
                10,
                m_display.Height() - 10,
                footer, m_deviceSettings.UIFont);

            m_renderer.DrawPage(
                10, 20,
                page);

            if (m_state == State::Menu)
            {
                DrawOverlayMenu();
            }
        });
}

void ReaderScreen::DrawOverlayMenu()
{
    const int panelX = 0;
    const int panelY = 0;

    const int panelW =
        m_display.Width();

    const int panelH =
        m_display.Height() / 2;

    m_display.FillRect(
        panelX,
        panelY,
        panelW,
        panelH);

    m_display.DrawRect(
        panelX,
        panelY,
        panelW,
        panelH);

    m_display.DrawLine(0,
                       panelY + 20,
                       panelW,
                       panelY + 20);

    m_display.DrawText(
        10,
        panelY + 5,
        "Reader Settings", m_deviceSettings.UIFont);

    int y = panelY + 40;

    for (size_t i = 0; i < static_cast<size_t>(OverlayMenuItem::Count); i++)
    {
        String text;

        switch (static_cast<OverlayMenuItem>(i))
        {
        case OverlayMenuItem::Font:
            text = "Font: [" + String(ToString(m_bookSettings.Font)) + "]";
            break;
        case OverlayMenuItem::FontSize:
            text = "Font Size: [" + String(m_bookSettings.FontSize) + "]";
            break;
        case OverlayMenuItem::Rotation:
            text = "Rotation: [" + String(m_deviceSettings.Orientation) + "]";
            break;
        case OverlayMenuItem::GoToPage:
            text = "Go To Page: [" + String(m_readerSettings.GoToPage) + "/%" + String(m_pageStarts.size() - 1) + "]";
            break;
        case OverlayMenuItem::Library:
            text = "Library";
            break;
        case OverlayMenuItem::Resume:
            text = "Resume";
            break;
        default:
            break;
        }

        if (i == m_readerSettings.SelectedMenuItem)
        {
            text = "> " + text;
        }

        m_display.DrawText(
            20,
            y,
            text, m_deviceSettings.UIFont);

        y += m_display.LineHeight(m_deviceSettings.UIFont) + 4;
    }
}

void ReaderScreen::MenuUp()
{
    if (m_readerSettings.SelectedMenuItem > 0)
    {
        m_readerSettings.SelectedMenuItem--;
    }
}

void ReaderScreen::MenuDown()
{
    if (m_readerSettings.SelectedMenuItem < static_cast<size_t>(OverlayMenuItem::Count) - 1)
    {
        m_readerSettings.SelectedMenuItem++;
    }
}

void ReaderScreen::MenuLeft()
{
    switch (
        static_cast<OverlayMenuItem>(
            m_readerSettings.SelectedMenuItem))
    {
    case OverlayMenuItem::Font:
    {
        int prevFontIndex = static_cast<int>(m_bookSettings.Font) - 1;
        // If index less than 0, then set font index to last index
        if (prevFontIndex < 0)
        {
            prevFontIndex = static_cast<int>(ReaderFont::Count) - 1;
        }
        m_bookSettings.Font = static_cast<ReaderFont>(prevFontIndex);
        UpdateLayout();
        break;
    }
    case OverlayMenuItem::FontSize:

        if (m_bookSettings.FontSize > 10)
        {
            m_bookSettings.FontSize -= 2;
        }
        UpdateLayout();
        break;

    case OverlayMenuItem::Rotation:

        if (m_deviceSettings.Orientation == 0)
        {
            m_deviceSettings.Orientation = 3;
        }
        else
        {
            m_deviceSettings.Orientation--;
        }
        UpdateLayout();
        break;

    case OverlayMenuItem::GoToPage:

        if (m_readerSettings.GoToPage > 1)
        {
            m_readerSettings.GoToPage--;
        }
        break;

    default:
        break;
    }
}

void ReaderScreen::MenuRight()
{
    switch (
        static_cast<OverlayMenuItem>(
            m_readerSettings.SelectedMenuItem))
    {
    case OverlayMenuItem::Font:
    {
        int nextFontIndex = static_cast<int>(m_bookSettings.Font) + 1;
        // If index greater than last index, then set font index to first index
        if (nextFontIndex >= static_cast<int>(ReaderFont::Count))
        {
            nextFontIndex = 0;
        }
        m_bookSettings.Font = static_cast<ReaderFont>(nextFontIndex);
        Serial.printf("Reader font changed to = %s\n", ToString(m_bookSettings.Font));
        UpdateLayout();
        break;
    }
    case OverlayMenuItem::FontSize:

        if (m_bookSettings.FontSize < 30)
        {
            m_bookSettings.FontSize += 2;
        }
        UpdateLayout();
        break;

    case OverlayMenuItem::Rotation:

        if (m_deviceSettings.Orientation >= 3)
        {
            m_deviceSettings.Orientation = 0;
        }
        else
        {
            m_deviceSettings.Orientation++;
        }
        UpdateLayout();
        break;

    case OverlayMenuItem::GoToPage:

        if (m_readerSettings.GoToPage < m_pageStarts.size() - 1)
        {
            m_readerSettings.GoToPage++;
        }
        break;

    default:
        break;
    }
}

void ReaderScreen::MenuSelect()
{
    switch (
        static_cast<OverlayMenuItem>(
            m_readerSettings.SelectedMenuItem))
    {
    case OverlayMenuItem::Resume: // Resume
        ToggleOverlayMenu();
        break;

    case OverlayMenuItem::GoToPage: // Go To Page
        GoToPage(m_readerSettings.GoToPage);
        ToggleOverlayMenu();
        break;
    default:
        break;
    }
}

void ReaderScreen::NextPage()
{
    if (m_readerSettings.CurrentPage + 1 < m_pageStarts.size())
    {
        m_readerSettings.CurrentPage++;
    }
}

void ReaderScreen::PreviousPage()
{
    m_readerSettings.CurrentPage--;
}

void ReaderScreen::GoToPage(size_t page)
{
    if (page < m_pageStarts.size())
    {
        m_readerSettings.CurrentPage = page;
    }
}