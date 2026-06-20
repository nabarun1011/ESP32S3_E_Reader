#include "UI/Screens/BaseScreen.hpp"

BaseScreen::BaseScreen(DeviceSettings &deviceSettings, IDisplay &display) : m_deviceSettings(deviceSettings), m_display(display) {}

void BaseScreen::Draw()
{
    Refresh();
}