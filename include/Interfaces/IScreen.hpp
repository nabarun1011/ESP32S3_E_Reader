// Interface for Screen states
#pragma once

class IScreen
{
public:
    virtual ~IScreen() = default;

    virtual void Enter() = 0;

    virtual void Exit() = 0;

    virtual void Update() = 0;

    virtual void Draw() = 0;
};