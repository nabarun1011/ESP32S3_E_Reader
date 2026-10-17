#include "UI/Screens/DeviceSettingsScreen.hpp"

DeviceSettingsScreen::DeviceSettingsScreen(
    IDisplay &display,
    DeviceSettings &deviceSettings)
    : BaseScreen(
          deviceSettings,
          display)
{
}

void DeviceSettingsScreen::Enter()
{
    BuildMenu();
    UpdateMenuValues();

    UpdateLayout();

    Refresh(true);
}

void DeviceSettingsScreen::Exit()
{
}

void DeviceSettingsScreen::Draw()
{
    DrawHeader("Device Settings");
    DrawMenu();
}

void DeviceSettingsScreen::DrawMenu()
{
    int y = m_layout.ContentY + 10;

    for (size_t i = 0; i < m_menu.Count(); ++i)
    {
        DrawMenuRow(
            m_menu.Item(i),
            m_menu.IsSelected(i),
            y);

        y += m_display.LineHeight(m_deviceSettings.UIFont);
    }
}

void DeviceSettingsScreen::DrawMenuRow(
    const MenuItem &item,
    bool selected,
    int y)
{
    DrawSelectionMarker(selected, y);

    DrawLeftText(
        20,
        y,
        item.Label);

    switch (item.Indicator)
    {
    case MenuIndicator::Adjustable:

        DrawAdjustableIndicator(
            m_display.Width() - 5,
            y,
            item.valueText);

        break;

    case MenuIndicator::Arrow:

        DrawArrowIndicator(
            m_display.Width() - 5,
            y);

        break;

    default:
        break;
    }
}

ScreenCommand DeviceSettingsScreen::HandleButton(
    Button button)
{
    switch (button)
    {
    case Button::Up:
        MoveUp();
        break;

    case Button::Down:
        MoveDown();
        break;

    case Button::Left:
        PreviousValue();
        UpdateMenuValues();
        UpdateLayout();
        break;

    case Button::Right:
        NextValue();
        UpdateMenuValues();
        UpdateLayout();
        break;

    case Button::Select:
        return Activate();

    case Button::Back:
        return {ScreenCommandType::OpenLibrary};

    default:
        break;
    }

    Refresh();

    return {};
}

void DeviceSettingsScreen::MoveUp()
{
    m_menu.MoveUp();
}

void DeviceSettingsScreen::MoveDown()
{
    m_menu.MoveDown();
}

void DeviceSettingsScreen::BuildMenu()
{
    m_menu.Clear();

    for (int i = 0; i < static_cast<int>(DeviceSettingItem::Count); i++)
    {
        switch (static_cast<DeviceSettingItem>(i))
        {
        case DeviceSettingItem::Font:
            m_menu.Add({"UI Font", "", MenuIndicator::Adjustable});
            break;
        case DeviceSettingItem::Orientation:
            m_menu.Add({"Orientation", "", MenuIndicator::Adjustable});
            break;
        case DeviceSettingItem::AutoSleep:
            m_menu.Add({"Auto Sleep", "", MenuIndicator::Adjustable});
            break;
        case DeviceSettingItem::Wifi:
            m_menu.Add({"WiFi", "", MenuIndicator::Adjustable});
            break;
        case DeviceSettingItem::USBMSC:
            m_menu.Add({"USB Mass Storage", "", MenuIndicator::Arrow});
            break;
        default:
            break;
        }
    }
}

void DeviceSettingsScreen::UpdateMenuValues()
{
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
    m_menu.Item(static_cast<size_t>(DeviceSettingItem::Orientation)).valueText = orientationText;
    m_menu.Item(static_cast<size_t>(DeviceSettingItem::Font)).valueText = ToString(m_deviceSettings.UIFont);
    m_menu.Item(static_cast<size_t>(DeviceSettingItem::AutoSleep)).valueText = std::to_string(m_deviceSettings.AutoSleep);
    m_menu.Item(static_cast<size_t>(DeviceSettingItem::Wifi)).valueText = m_deviceSettings.WifiEnabled ? "On" : "Off";
}

void DeviceSettingsScreen::PreviousValue()
{
    switch (static_cast<DeviceSettingItem>(m_menu.GetSelectedIndex()))
    {
    case DeviceSettingItem::Orientation:
    {
        if (m_deviceSettings.Orientation <= 0)
        {
            m_deviceSettings.Orientation = 3;
        }
        else
        {
            m_deviceSettings.Orientation--;
        }
    }
    break;

    case DeviceSettingItem::Font:
    {
        int nextFOntIndex = static_cast<int>(m_deviceSettings.UIFont) - 1;
        // If index greater than last index, then set font index to first index
        if (nextFOntIndex < 0)
        {
            nextFOntIndex = static_cast<int>(ReaderFont::Count) - 1;
        }
        m_deviceSettings.UIFont = static_cast<ReaderFont>(nextFOntIndex);
        Serial.printf("Reader font changed to = %s\n", ToString(m_deviceSettings.UIFont));

        break;
    }
    default:
        break;
    }

    
}

void DeviceSettingsScreen::NextValue()
{
    switch (static_cast<DeviceSettingItem>(m_menu.GetSelectedIndex()))
    {
    case DeviceSettingItem::Orientation:
    {
        if (m_deviceSettings.Orientation >= 3)
            m_deviceSettings.Orientation = 0;
        else
            m_deviceSettings.Orientation++;
    }
    break;

    case DeviceSettingItem::Font:
    {
        int prevFontIndex = static_cast<int>(m_deviceSettings.UIFont) + 1;
        // If index greater than last index, then set font index to first index
        if (prevFontIndex >= static_cast<int>(ReaderFont::Count))
        {
            prevFontIndex = 0;
        }
        m_deviceSettings.UIFont = static_cast<ReaderFont>(prevFontIndex);
        Serial.printf("Reader font changed to = %s\n", ToString(m_deviceSettings.UIFont));

        break;
    }
    default:
        break;
    }
}

ScreenCommand DeviceSettingsScreen::Activate()
{
    return {};
}
