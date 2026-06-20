// Interface for Screen states
#pragma once

#include "Platform/Input/Button.hpp"

class IScreen
{
public:
    virtual ~IScreen() = default;

    virtual void Enter() = 0;

    virtual void Exit() = 0;

    virtual void HandleButton(Button button) = 0;

    virtual void Draw() = 0;

    virtual void Refresh() = 0;
};