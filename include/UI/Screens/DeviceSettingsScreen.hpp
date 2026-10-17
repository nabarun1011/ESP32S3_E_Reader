#pragma once

#include "UI/Screens/BaseScreen.hpp"
#include "UI/Layout/DeviceSettingsLayout.hpp"
#include "UI/Components/Menu/Menu.hpp"

class DeviceSettingsScreen : public BaseScreen<DeviceSettingsLayout>
{
public:
    DeviceSettingsScreen(
        IDisplay &display,
        DeviceSettings &deviceSettings);

    void Enter() override;

    void Exit() override;

    void Draw() override;

    ScreenCommand HandleButton(
        Button button) override;

private:
    void DrawMenu();

    void DrawMenuRow(
        const MenuItem &item,
        bool selected,
        int y);

    void BuildMenu();

    void MoveUp();

    void MoveDown();

    void PreviousValue();

    void NextValue();

    ScreenCommand Activate();

    void UpdateMenuValues();

    enum class DeviceSettingItem
    {
        Font,
        Orientation,
        AutoSleep,
        Wifi,
        USBMSC,
        Count
    };

    // MenuItem GetMenuText(DeviceSettingItem item) const;

private:
    Menu m_menu;
};