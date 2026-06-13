#include <DeviceSettings.h>
#include <Preferences.h>
#include <Pins.h>
#include <InputActions.h>
#include <UIRenderer.h>
#include <WiFi.h>

Preferences preferenceManager;
DeviceSettingsMenu DeviceSettingsMenuInstance;

void DeviceSettingsMenu::Init()
{
    settings.UIFontFamily = Font_Sans;
    settings.UIFontSize = 0;
    settings.isWifiEnabled = false;
    settings.isUSBMSCEnabled = false;
    settings.orientation = Portrait;
    Load();
}

void DeviceSettingsMenu::Load()
{
    preferenceManager.begin("settings", true);
    settings.UIFontFamily = (FontFamily)preferenceManager.getInt("UIFontFamily", Font_Sans);
    settings.UIFontSize = preferenceManager.getInt("UIFontSize", 0);
    settings.isWifiEnabled = preferenceManager.getBool("isWifiEnabled", false);
    if (settings.isWifiEnabled)
    {
        WiFi.mode(WIFI_STA);
    }
    else
    {
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
    }
    // settings.isUSBMSCEnabled = preferenceManager.getBool("isUSBMSCEnabled", false);
    settings.orientation = (Orientation)preferenceManager.getInt("orientation", Portrait);
    preferenceManager.end();
}

void DeviceSettingsMenu::Save()
{
    preferenceManager.begin("settings", true);
    preferenceManager.putInt("UIFontFamily", settings.UIFontFamily);
    preferenceManager.putInt("UIFontSize", settings.UIFontSize);
    preferenceManager.putBool("isWifiEnabled", settings.isWifiEnabled);
    preferenceManager.putInt("orientation", settings.orientation);
    preferenceManager.end();
}

void DeviceSettingsMenu::Render()
{
    display.setTextWrap(true);
    display.setFont(GetCurrentFont());

    int y = headerHeight + 20;

    for (int i = 0; i < static_cast<int>(DeviceSettingsMenuItem_Count); i++)
    {
        display.setCursor(10, y + i * 30);
        if (i == selectedIndex)
        {
            display.print(">");
        }
        else
        {
            display.print(" ");
        }

        switch (static_cast<DeviceSettingsMenuItem>(i))
        {
        case DeviceSettingsMenuItem_FontFamily:
            display.println("Font: <" + fontFamilyNames[static_cast<int>(settings.UIFontFamily)] + ">");
            break;
        case DeviceSettingsMenuItem_FontSize:
            display.println("Font Size: <" + String(settings.UIFontSize) + ">");
            break;
        case DeviceSettingsMenuItem_Orientation:
            display.println("Orientation: <" + orientationNames[settings.orientation] + ">");
            break;
        case DeviceSettingsMenuItem_Wifi:
            display.println(String("Wifi: <") + (settings.isWifiEnabled ? "Enabled" : "Disabled") + ">" + String(settings.isWifiEnabled ? "<" : ""));
            break;
        case DeviceSettingsMenuItem_USBMSC:
            display.println(String("USB MSC: <") + (settings.isUSBMSCEnabled ? "Enabled" : "Disabled") + ">" + String(settings.isUSBMSCEnabled ? "<" : ""));
            break;
        default:
            break;
        }
    }
}

DeviceSettingsResult DeviceSettingsMenu::HandleInput(InputAction action)
{
    Serial.println("Setting Input handling");
    switch (action)
    {
    case Action_UP:
        selectedIndex--;
        if (selectedIndex < 0)
            selectedIndex = static_cast<int>(DeviceSettingsMenuItem_Count) - 1;
        break;
    case Action_DOWN:
        selectedIndex++;
        if (selectedIndex > static_cast<int>(DeviceSettingsMenuItem_Count) - 1)
            selectedIndex = 0;
        break;
    case Action_SELECT:
        switch (static_cast<DeviceSettingsMenuItem>(selectedIndex))
        {
        case DeviceSettingsMenuItem_Wifi:
            if (settings.isWifiEnabled)
            {
                return DeviceSettingsResult_WifiMenu;
            }
            break;
        case DeviceSettingsMenuItem_USBMSC:
            if (settings.isUSBMSCEnabled)
            {
                return DeviceSettingsResult_USBMSCMenu;
            }
            break;
        default:
            break;
        }
        break;
    case Action_BACK:
        Save();
        return DeviceSettingsResult_BackToLibrary;
        break;
    case Action_LEFT:
        switch (static_cast<DeviceSettingsMenuItem>(selectedIndex))
        {
        case DeviceSettingsMenuItem_FontFamily:
            settings.UIFontFamily = static_cast<FontFamily>(settings.UIFontFamily - 1 < 0 ? Font_Count - 1 : settings.UIFontFamily - 1);
            break;
        case DeviceSettingsMenuItem_FontSize:
            settings.UIFontSize--;
            if (settings.UIFontSize < 0)
                settings.UIFontSize = 1;
            break;
        case DeviceSettingsMenuItem_Orientation:
            settings.orientation = static_cast<Orientation>(settings.orientation - 1 < 0 ? Orientation_Count - 1 : settings.orientation - 1);
            break;
        case DeviceSettingsMenuItem_Wifi:
            settings.isWifiEnabled = !settings.isWifiEnabled;
            if (!settings.isWifiEnabled)
            {
                WiFi.disconnect(true);
                WiFi.mode(WIFI_OFF);
            }
            else
            {
                WiFi.mode(WIFI_STA);
            }
            break;
        case DeviceSettingsMenuItem_USBMSC:
            settings.isUSBMSCEnabled = !settings.isUSBMSCEnabled;
            break;
        default:
            break;
        }
        break;
    case Action_RIGHT:
        switch (static_cast<DeviceSettingsMenuItem>(selectedIndex))
        {
        case DeviceSettingsMenuItem_FontFamily:
            settings.UIFontFamily = static_cast<FontFamily>(settings.UIFontFamily + 1 >= Font_Count ? 0 : settings.UIFontFamily + 1);
            break;
        case DeviceSettingsMenuItem_FontSize:
            settings.UIFontSize++;
            if (settings.UIFontSize > 1)
            {
                settings.UIFontSize = 0;
            }
            break;
        case DeviceSettingsMenuItem_Orientation:
            settings.orientation = static_cast<Orientation>(settings.orientation + 1 >= Orientation_Count ? 0 : settings.orientation + 1);
            break;
        case DeviceSettingsMenuItem_Wifi:
            settings.isWifiEnabled = !settings.isWifiEnabled;
            if (!settings.isWifiEnabled)
            {
                WiFi.disconnect(true);
                WiFi.mode(WIFI_OFF);
            }
            else
            {
                WiFi.mode(WIFI_STA);
            }
            break;
        case DeviceSettingsMenuItem_USBMSC:
            settings.isUSBMSCEnabled = !settings.isUSBMSCEnabled;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return DeviceSettingsResult_None;
}