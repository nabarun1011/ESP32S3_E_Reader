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

bool InputManager::Update()
{
    Button current =
        ReadButtons();

    if (current != m_candidateButton)
    {
        m_candidateButton = current;

        m_lastChangeMs =
            millis();

        return false;
    }

    if ((millis() - m_lastChangeMs) < DebounceMs)
    {
        return false;
    }

    if (m_candidateButton == m_lastStableButton)
    {
        return false;
    }

    m_lastStableButton =
        m_candidateButton;

    if (m_lastStableButton != Button::None)
    {
        return true;
    }

    return false;
}

Button InputManager::GetLastStableButton()
{
    return m_lastStableButton;
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