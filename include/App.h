#pragma once

#include <AppState.h>
#include <InputActions.h>

class App
{
private:
    AppState currentState;

public:
    void Begin();
    void Update();
    void ChangeState(AppState newState);
    void HandleInput(InputAction action);
    void ReadButtonPresses();
    void RenderCurrentScreen();
};



extern App appInstance;