#include <Arduino.h>
#include "Platform/Input/InputManager.hpp"

InputManager::InputManager(
    ScreenManager &screenManager)
    : m_screenManager(screenManager)
{
}

bool InputManager::Init()
{
    return true;
}

void InputManager::Update()
{
    Button current =
        ReadButtons();

    if (current != m_candidateButton)
    {
        m_candidateButton = current;

        m_lastChangeMs =
            millis();

        return;
    }

    if ((millis() - m_lastChangeMs) < DebounceMs)
    {
        return;
    }

    if (m_candidateButton == m_lastStableButton)
    {
        return;
    }

    m_lastStableButton =
        m_candidateButton;

    if (m_lastStableButton != Button::None)
    {
        // Serial.printf("Button: %d\n", static_cast<uint32_t>(m_lastStableButton));
        m_screenManager
            .HandleButton(
                m_lastStableButton);
    }
}

Button InputManager::ReadButtons()
{
    Button button =
        ReadDirectionButton();

    if (button != Button::None)
    {
        return button;
    }

    return ReadActionButton();
}

Button InputManager::ReadDirectionButton()
{
    int value = analogRead(Config::NavButton_Pin);

    if (value > 3800)
    {
        return Button::None;
    }
    else
    {
        if (value < 60)
        {
            return Button::Up;
        }
        else if (value < 120)
        {
            return Button::Down;
        }
        else if (value < 250)
        {
            return Button::Left;
        }
        else
        {
            return Button::Right;
        }
    }
    return Button::None;
}

Button InputManager::ReadActionButton()
{
    int value = analogRead(Config::ActionBUtton_Pin);

    if (value > 3800)
    {
        return Button::None;
    }
    else
    {
        if (value < 100)
        {
            return Button::Select;
        }
        else
        {
            return Button::Back;
        }
    }
    return Button::None;
}