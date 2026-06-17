#pragma once

#include <Arduino.h>
#include <functional>

class IDisplay
{
public:
    virtual void Render(const std::function<void()> &drawFunc) = 0;

    virtual ~IDisplay() = default;

    virtual int Width() const = 0;
    virtual int Height() const = 0;

    virtual int LineHeight() const = 0;

    virtual int MeasureTextWidth(const String &text) = 0;

    virtual bool Init() = 0;

    virtual void SetTextSize(int size) = 0;

    virtual void SetRotation(uint8_t rotation) = 0;

    virtual void DrawText(
        int x,
        int y,
        const String &text) = 0;
};