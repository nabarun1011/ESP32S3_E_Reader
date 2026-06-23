#pragma once

#include "Interfaces/IScreen.hpp"
#include "UI/ScreenCommand.hpp"

class ScreenManager
{
public:
    void SetScreen(IScreen *screen);

    ScreenCommand HandleButton(Button button);

    void Draw();

private:
    IScreen *m_currentScreen = nullptr;
};