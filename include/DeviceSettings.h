#pragma once
#include <FontManager.h>
#include <InputActions.h>

enum Orientation
{
    Landscape,
    PortraitInverted,
    LandscapeInverted,
    Portrait,

    Orientation_Count
};

const String orientationNames[4] = {"Landscape", "Portrait Inverted", "Landscape Inverted", "Portrait"};

struct DeviceSettings
{
    FontFamily UIFontFamily;
    int UIFontSize;
    Orientation orientation;
    bool isWifiEnabled;
    bool isUSBMSCEnabled;
};

enum DeviceSettingsMenuItem
{
    DeviceSettingsMenuItem_FontFamily,
    DeviceSettingsMenuItem_FontSize,
    DeviceSettingsMenuItem_Orientation,
    DeviceSettingsMenuItem_Wifi,
    DeviceSettingsMenuItem_USBMSC,

    DeviceSettingsMenuItem_Count
};

enum DeviceSettingsResult
{
    DeviceSettingsResult_None,
    DeviceSettingsResult_BackToLibrary,
    DeviceSettingsResult_WifiMenu,
    DeviceSettingsResult_USBMSCMenu
};

class DeviceSettingsMenu
{
private:
    int selectedIndex = 0;

public:
    DeviceSettings settings;
    void Init();
    void Save();
    void Load();
    void Render();
    DeviceSettingsResult HandleInput(InputAction action);
};

extern DeviceSettingsMenu DeviceSettingsMenuInstance;
