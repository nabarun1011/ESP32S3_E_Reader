#pragma once

#include <string>

enum class MenuIndicator : uint8_t
{
    None = 0,
    Adjustable = 1 << 0,
    Arrow = 1 << 1
};

struct MenuItem
{
    std::string Label;
    std::string valueText;

    MenuIndicator Indicator = MenuIndicator::None;

    bool Enabled = true;
};