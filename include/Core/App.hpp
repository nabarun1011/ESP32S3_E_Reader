#pragma once

#include "Platform/Storage/SDStorage.hpp"
#include "Platform/Display/DisplayManager.hpp"
#include "Domain/Library/LibraryScanner.hpp"

class App
{
private:
    SDStorage m_storage;

    LibraryScanner m_library;

    DisplayManager m_display;

public:
    App();
    bool Init();
    void Update();
    // void ChangeState(AppState newState);
    // void HandleInput(InputAction action);
    // void ReadButtonPresses();
    // void RenderCurrentScreen();
};
