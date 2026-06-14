#pragma once

#include "Platform/Storage/SDStorage.hpp"
#include "Platform/Display/DisplayManager.hpp"

class App
{
private:
    SDStorage m_storage;

    DisplayManager m_display;

public:
    bool Init();
    void Update();
    // void ChangeState(AppState newState);
    // void HandleInput(InputAction action);
    // void ReadButtonPresses();
    // void RenderCurrentScreen();
};
