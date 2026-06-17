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

    void Render(const std::function<void()> &drawFunc) override;

    int Width() const override;

    int Height() const override;

    int LineHeight() const override;

    int MeasureTextWidth(const String &text) override;

    bool Init() override;

    void DrawText(int x, int y, const String &text) override;
};