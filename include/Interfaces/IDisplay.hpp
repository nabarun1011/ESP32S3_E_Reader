#pragma once

#include <Arduino.h>
#include <functional>
#include <Domain/Reader/ReaderFont.hpp>

class IDisplay
{
public:
    virtual void Render(const std::function<void()> &drawFunc) = 0;

    virtual ~IDisplay() = default;

    virtual int Width() const = 0;
    virtual int Height() const = 0;

    virtual int LineHeight(ReaderFont font) const = 0;

    virtual int MeasureTextWidth(const String &text, ReaderFont font) = 0;

    virtual bool Init() = 0;

    // virtual void SetFont(const ReaderFont font) = 0;

    virtual void SetTextSize(int size) = 0;

    virtual void SetRotation(uint8_t rotation) = 0;

    virtual void DrawText(
        int x,
        int y,
        const String &text,
        ReaderFont font) = 0;

    virtual void DrawRect(
        int x,
        int y,
        int width,
        int height) = 0;

    virtual void FillRect(
        int x,
        int y,
        int width,
        int height) = 0;

    virtual void DrawLine(int x1, int y1, int x2, int y2) = 0;
};