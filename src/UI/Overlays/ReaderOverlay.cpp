#include "UI/Overlays/ReaderOverlay.hpp"

static const char *TAG = "ReaderOverlay";

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

void ReaderOverlay::Show()
{
    BaseOverlay::Show();
    BuildMenu();
}

void ReaderOverlay::Draw()
{
    const int panelX = 0;
    const int panelY = m_display.LineHeight(m_deviceSettings.UIFont) * 0.8;

    const int panelW =
        m_display.Width();

    const int panelH =
        m_display.Height() / 2;

    m_display.FillRect(
        panelX,
        0,
        panelW,
        panelH);

    m_display.DrawRect(
        panelX,
        0,
        panelW,
        panelH);

    m_display.DrawLine(0,
                       panelY + 20,
                       panelW,
                       panelY + 20);

    m_display.DrawText(
        10,
        panelY,
        "Reader Settings", m_deviceSettings.UIFont);

    int y = panelY + 40;

    for (size_t i = 0; i < m_menu.Count(); ++i)
    {
        DrawMenuRow(
            m_menu.Item(i),
            m_menu.IsSelected(i),
            y);

        y += m_display.LineHeight(m_deviceSettings.UIFont);
    }
}

void ReaderOverlay::DrawMenuRow(const MenuItem &item, bool selected, int y)
{
    if (selected)
    {
        m_display.DrawText(
            5,
            y,
            ">",
            m_deviceSettings.UIFont);
    }

    // Draw left label text
    m_display.DrawText(
        20,
        y,
        item.Label,
        m_deviceSettings.UIFont);

    // Draw right part
    switch (item.Indicator)
    {
    case MenuIndicator::Adjustable:
        m_display.DrawTextRight(
            m_display.Width() - 5,
            y,
            "<" + item.valueText + ">",
            m_deviceSettings.UIFont);

        break;

    case MenuIndicator::Arrow:
        m_display.DrawTextRight(
            m_display.Width() - 5,
            y,
            ">",
            m_deviceSettings.UIFont);
        break;

    default:
        break;
    }
}

void ReaderOverlay::HandleButton(Button button)
{
    switch (button)
    {
    case Button::Up:
        m_menu.MoveUp();
        UpdateMenuValues();
        break;
    case Button::Down:
        m_menu.MoveDown();
        UpdateMenuValues();
        break;
    case Button::Left:
        MenuLeft();
        UpdateMenuValues();
        break;
    case Button::Right:
        MenuRight();
        UpdateMenuValues();
        break;
    case Button::Select:
        MenuSelect();
        UpdateMenuValues();
        break;
    case Button::Back:
        Hide();
        break;
    default:
        break;
    }
}

void ReaderOverlay::MenuLeft()
{
    switch (
        static_cast<OverlayMenuItem>(
            m_menu.GetSelectedIndex()))
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

    case OverlayMenuItem::Orientation:

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
            m_menu.GetSelectedIndex()))
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
        Serial.printf("Reader font changed to = %s\n", ToString(m_bookSettings.Font).c_str());
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

    case OverlayMenuItem::Orientation:

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

void ReaderOverlay::BuildMenu()
{
    Serial.printf("%s : BuildMenu\n", TAG);
    m_menu.Clear();

    for (int i = 0;
         i < static_cast<int>(OverlayMenuItem::Count);
         ++i)
    {
        m_menu.Add(GetMenuItem(
            static_cast<OverlayMenuItem>(i)));
    }

    UpdateMenuValues();
}

void ReaderOverlay::UpdateMenuValues()
{
    m_menu.Item(static_cast<size_t>(OverlayMenuItem::Font)).valueText = ToString(m_bookSettings.Font);

    m_menu.Item(static_cast<size_t>(OverlayMenuItem::FontSize)).valueText = std::to_string(m_bookSettings.FontSize);

    std::string orientationText = "";
    switch (m_deviceSettings.Orientation)
    {
    case 0:
        orientationText = "Landscape";
        break;
    case 1:
        orientationText = "Inverted Portrait";
        break;
    case 2:
        orientationText = "Inverted Landscape";
        break;
    case 3:
        orientationText = "Portrait";
        break;
    }
    m_menu.Item(static_cast<size_t>(OverlayMenuItem::Orientation)).valueText = orientationText;

    m_menu.Item(static_cast<size_t>(OverlayMenuItem::GoToPage)).valueText = std::to_string(m_readerSettings.GoToPage) +
                                                                            "/" +
                                                                            std::to_string(m_pages.size() - 1);
}

MenuItem ReaderOverlay::GetMenuItem(
    OverlayMenuItem item)
{
    switch (item)
    {
    case OverlayMenuItem::Font:

        return {
            "Font",
            "",
            MenuIndicator::Adjustable};

    case OverlayMenuItem::FontSize:

        return {
            "Font Size",
            "",
            MenuIndicator::Adjustable};

    case OverlayMenuItem::Orientation:

        return {
            "Orientation",
            "",
            MenuIndicator::Adjustable};

    case OverlayMenuItem::GoToPage:

        return {
            "Go To Page",
            "",
            MenuIndicator::Adjustable};
    case OverlayMenuItem::Resume:
        return {
            "Resume",
            "",
            MenuIndicator::Arrow};
    case OverlayMenuItem::Library:
        return {
            "Library",
            "",
            MenuIndicator::Arrow};
    }
    return {};
}

void ReaderOverlay::MenuSelect()
{
    switch (
        static_cast<OverlayMenuItem>(
            m_menu.GetSelectedIndex()))
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