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
    m_selectedItem = 0;

    UpdateLayout();

    Refresh(true);
}

// void DeviceSettingsScreen::UpdateLayout()
// {
//     m_layout.HeaderHeight = m_display.LineHeight(m_deviceSettings.UIFont);
//     m_layout.ContentY = m_layout.HeaderHeight + m_display.LineHeight(m_deviceSettings.UIFont) * 0.8;
//     m_layout.FooterHeight = m_display.LineHeight(m_deviceSettings.UIFont);
//     m_layout.ContentHeight = m_display.Height() - m_layout.FooterHeight - m_layout.HeaderHeight;
// }

void DeviceSettingsScreen::Exit()
{
}

void DeviceSettingsScreen::Draw()
{
    DrawHeader("Device Settings");
    DrawMenu();
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
        break;

    case Button::Right:
        NextValue();
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

String DeviceSettingsScreen::GetMenuText(
    DeviceSettingItem item) const
{
    switch (item)
    {
    case DeviceSettingItem::Orientation:
        return "Orientation: [" +
               String(m_deviceSettings.Orientation) +
               "]";

    case DeviceSettingItem::Font:
        return "UI Font: [" +
               String(ToString(m_deviceSettings.UIFont)) +
               "]";

    case DeviceSettingItem::USBMSC:
        return "USB Mass Storage";

    default:
        return "";
    }
}

void DeviceSettingsScreen::DrawMenu()
{
    int y = m_layout.ContentY + 10;

    for (size_t i = 0;
         i < static_cast<size_t>(DeviceSettingItem::Count);
         ++i)
    {
        String text = GetMenuText(static_cast<DeviceSettingItem>(i));

        if (i == m_selectedItem)
        {
            text = "> " + text;
        }

        m_display.DrawText(
            20,
            y,
            text,
            m_deviceSettings.UIFont);

        y +=
            m_display.LineHeight(
                m_deviceSettings.UIFont) +
            4;
    }
}

void DeviceSettingsScreen::MoveUp()
{
    if (m_selectedItem > 0)
    {
        m_selectedItem--;
    }
    else
    {
        m_selectedItem = static_cast<size_t>(DeviceSettingItem::Count) - 1;
    }
}

void DeviceSettingsScreen::MoveDown()
{
    if (m_selectedItem < static_cast<size_t>(DeviceSettingItem::Count) - 1)
    {
        m_selectedItem++;
    }
    else
    {
        m_selectedItem = 0;
    }
}

void DeviceSettingsScreen::PreviousValue()
{
}

void DeviceSettingsScreen::NextValue()
{
}

ScreenCommand DeviceSettingsScreen::Activate()
{
    return {};
}
