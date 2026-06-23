#pragma once

#include <cstdint>
#include "UI/ScreenManager.hpp"
#include "Platform/Input/Button.hpp"
#include "Core/Config.hpp"

class InputManager
{
public:
    InputManager(
        ScreenManager &screenManager);

    bool Init();

    bool Update();

    Button GetLastStableButton();

private:
    Button ReadDirectionButton();
    Button ReadActionButton();
    Button ReadButtons();

private:
    ScreenManager &m_screenManager;

    Button m_lastStableButton =
        Button::None;

    Button m_candidateButton =
        Button::None;

    uint32_t m_lastChangeMs = 0;

    static constexpr uint32_t
        DebounceMs = 40;
};