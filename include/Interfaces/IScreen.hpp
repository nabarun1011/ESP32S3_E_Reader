// Interface for Screen states
#pragma once

#include "Platform/Input/Button.hpp"
#include "UI/ScreenCommand.hpp"

struct RefreshRequest
{
    bool Full = false;

    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

class IScreen
{
public:
    virtual ~IScreen() = default;

    virtual void Enter() = 0;

    virtual void Exit() = 0;

    virtual ScreenCommand HandleButton(Button button) = 0;

    virtual void Refresh(const RefreshRequest &request) = 0;

    virtual void Refresh(bool full = true) = 0;

    virtual void Draw() = 0;
};