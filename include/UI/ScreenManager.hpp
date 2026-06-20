#pragma once

#include "Interfaces/IScreen.hpp"

class ScreenManager
{
public:
    void SetScreen(IScreen *screen);

    void HandleButton(Button button);

    void Draw();

private:
    IScreen *m_currentScreen = nullptr;
};