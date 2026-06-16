#pragma once

#include "Interfaces/IScreen.hpp"

class ScreenManager
{
public:
    void SetScreen(
        IScreen *screen);

    void Update();

    void Draw();

private:
    IScreen *m_current = nullptr;
};