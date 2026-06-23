#pragma once

#include "Interfaces/IDisplay.hpp"
#include "GxEPD2_BW.h"
#include "Core/Config.hpp"
#include <SPI.h>

class EPDDisplay : public IDisplay
{

private:
    GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT> m_display;

public:
    explicit EPDDisplay();

    void RenderFull(const std::function<void()> &drawFunc) override;

    void RenderPartial(int x, int y, int width, int height, const std::function<void()> &drawFunc) override;

    int Width() const override;

    int Height() const override;

    int LineHeight(ReaderFont font) const override;

    int MeasureTextWidth(const String &text, ReaderFont font) override;

    // void SetFont(const ReaderFont font) override;

    void SetTextSize(int size) override;

    bool Init() override;

    void SetRotation(uint8_t rotation) override;

    void DrawText(int x, int y, const String &text, ReaderFont font) override;

    void DrawRect(int x, int y, int width, int height) override;

    void FillRect(int x, int y, int width, int height) override;

    void DrawLine(int x1, int y1, int x2, int y2) override;

    const GFXfont *GetFont(ReaderFont font) const;
};