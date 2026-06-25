#include "UI/Overlays/ReaderOverlay.hpp"

ReaderOverlay::ReaderOverlay(
    IDisplay &display,
    DeviceSettings &deviceSettings,
    BookSettings &bookSettings,
    ReaderSettings &readerSettings,
    std::function<void()> updateLayout,
    std::function<void(size_t)> goToPage,
    std::vector<PageIndexedEntry> &pages)
    : m_display(display),
      m_deviceSettings(deviceSettings),
      m_bookSettings(bookSettings),
      m_readerSettings(readerSettings),
      m_updateLayout(updateLayout),
      m_goToPage(goToPage),
      m_pages(pages)
{
}

void ReaderOverlay::Draw()
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
            text = "Go To Page: [" + String(m_readerSettings.GoToPage) + "/" + String(m_pages.size() - 1) + "]";
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

void ReaderOverlay::HandleButton(Button button)
{
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
        Hide();
        break;
    default:
        break;
    }
}

void ReaderOverlay::MenuUp()
{
    if (m_readerSettings.SelectedMenuItem > 0)
    {
        m_readerSettings.SelectedMenuItem--;
    }
}

void ReaderOverlay::MenuDown()
{
    if (m_readerSettings.SelectedMenuItem < static_cast<size_t>(OverlayMenuItem::Count) - 1)
    {
        m_readerSettings.SelectedMenuItem++;
    }
}

void ReaderOverlay::MenuLeft()
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
        m_updateLayout();
        break;
    }
    case OverlayMenuItem::FontSize:

        if (m_bookSettings.FontSize > 10)
        {
            m_bookSettings.FontSize -= 2;
        }
        m_updateLayout();
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
        m_updateLayout();
        break;

    case OverlayMenuItem::GoToPage:

        if (m_readerSettings.GoToPage > 1)
        {
            m_readerSettings.GoToPage--;
        }
        else
        {
            m_readerSettings.GoToPage = m_pages.size() - 1;
        }
        break;

    default:
        break;
    }
}

void ReaderOverlay::MenuRight()
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
        m_updateLayout();
        break;
    }
    case OverlayMenuItem::FontSize:

        if (m_bookSettings.FontSize < 30)
        {
            m_bookSettings.FontSize += 2;
        }
        m_updateLayout();
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
        m_updateLayout();
        break;

    case OverlayMenuItem::GoToPage:

        if (m_readerSettings.GoToPage < m_pages.size() - 1)
        {
            m_readerSettings.GoToPage++;
        }
        else
        {
            m_readerSettings.GoToPage = 0;
        }
        break;

    default:
        break;
    }
}

void ReaderOverlay::MenuSelect()
{
    switch (
        static_cast<OverlayMenuItem>(
            m_readerSettings.SelectedMenuItem))
    {
    case OverlayMenuItem::Resume: // Resume
        Hide();
        break;

    case OverlayMenuItem::GoToPage: // Go To Page
        m_goToPage(m_readerSettings.GoToPage);
        Hide();
        break;  
    default:
        break;
    }
}