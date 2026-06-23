// Interface for Screen states
#pragma once

#include "Platform/Input/Button.hpp"
#include "UI/ScreenCommand.hpp"

enum class RefreshMode
{
    None,
    Partial,
    Full
};

class IScreen
{
public:
    virtual ~IScreen() = default;

    virtual void Enter() = 0;

    virtual void Exit() = 0;

    virtual ScreenCommand HandleButton(Button button) = 0;


    virtual void Refresh(RefreshMode mode=RefreshMode::Partial) = 0;
};