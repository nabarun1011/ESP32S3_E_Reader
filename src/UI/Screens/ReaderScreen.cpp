#include "UI/Screens/ReaderScreen.hpp"

ReaderScreen::ReaderScreen(
    TextDocument &document,
    IDisplay &display,
    DeviceSettings &deviceSettings)
    : m_document(document),
      m_display(display),
      m_renderer(display),
      m_deviceSettings(deviceSettings)
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

    m_readerSettings.FontSize = m_bookSettings.FontSize;

    m_readerSettings.Font = m_bookSettings.Font;    

    // m_readerSettings.CurrentPage = m_bookSettings.LastPage;

    m_readerSettings.GoToPage = m_readerSettings.CurrentPage;

    ApplyLayoutSettings();

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

void ReaderScreen::ApplyLayoutSettings()
{
    m_display.SetRotation(m_deviceSettings.Orientation);
    m_display.SetTextSize(m_readerSettings.FontSize);
    RebuildPageIndex();
}

void ReaderScreen::Exit()
{
}

void ReaderScreen::Update()
{
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
                startLine);

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
    if (m_state == State::Menu)
    {
        m_state = State::Reading;
    }
    else
    {
        m_state = State::Menu;
    }
}

void ReaderScreen::Draw()
{
    m_display.SetTextSize(m_readerSettings.FontSize);

    Page page =
        m_renderer.BuildPage(
            m_text,
            m_pageStarts[m_readerSettings.CurrentPage]);

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
                footer);

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
    int y = 40;

    for (size_t i = 0; i < static_cast<size_t>(OverlayMenuItem::Count); i++)
    {
        String text;

        switch (static_cast<OverlayMenuItem>(i))
        {
        case OverlayMenuItem::Font:
            text = "Font: [Dummy Font]";
            break;
        case OverlayMenuItem::FontSize:
            text = "Font Size: [" + String(m_readerSettings.FontSize) + "]";
            break;
        case OverlayMenuItem::Rotation:
            text = "Rotation: [" + String(m_deviceSettings.Orientation) + "]";
            break;
        case OverlayMenuItem::GoToPage:
            text = "Go To Page: [%u/%u]", m_readerSettings.CurrentPage, m_pageStarts.size() - 1;
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
            text);

        y += 30;
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
    case OverlayMenuItem::FontSize:

        if (m_readerSettings.FontSize > 10)
        {
            m_readerSettings.FontSize -= 2;
        }
        ApplyLayoutSettings();
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
        ApplyLayoutSettings();
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
    case OverlayMenuItem::FontSize:

        if (m_readerSettings.FontSize < 30)
        {
            m_readerSettings.FontSize += 2;
        }
        ApplyLayoutSettings();
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
        ApplyLayoutSettings();
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
        m_state = State::Reading;
        break;

    case OverlayMenuItem::GoToPage: // Go To Page
        GoToPage(m_readerSettings.GoToPage);
        m_state = State::Reading;
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