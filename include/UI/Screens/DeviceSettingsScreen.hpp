#pragma once

#include "UI/Screens/BaseScreen.hpp"
#include "UI/Layout/DeviceSettingsLayout.hpp"

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

    void MoveUp();

    void MoveDown();

    void PreviousValue();

    void NextValue();

    ScreenCommand Activate();

    enum class DeviceSettingItem
    {
        Font,
        Orientation,
        USBMSC,
        Count
    };

    String GetMenuText(DeviceSettingItem item) const;

private:
    size_t m_selectedItem = 0;
};