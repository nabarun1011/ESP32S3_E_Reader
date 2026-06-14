#pragma once

class IDisplay
{
public:
    virtual ~IDisplay() = default;
    virtual bool Init() = 0;
    virtual void Clear() = 0;
};