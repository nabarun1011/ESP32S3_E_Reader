#pragma once

#include "Interfaces/IScreen.hpp"
#include "Domain/Settings/DeviceSettings.hpp"
#include "Interfaces/IDisplay.hpp"

class BaseScreen : public IScreen
{
public:
    BaseScreen(DeviceSettings &deviceSettings, IDisplay &display);
protected:
    DeviceSettings &m_deviceSettings;
    IDisplay &m_display;
};