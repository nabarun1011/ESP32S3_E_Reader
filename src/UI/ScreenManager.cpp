#include "UI/ScreenManager.hpp"

void ScreenManager::SetScreen(
    IScreen *screen)
{
    if (m_currentScreen)
    {
        m_currentScreen->Exit();
    }

    m_currentScreen = screen;

    if (m_currentScreen)
    {
        m_currentScreen->Enter();
    }
}

void ScreenManager::HandleButton(
    Button button)
{
    if (m_currentScreen)
    {
        m_currentScreen->HandleButton(
            button);
    }
}

void ScreenManager::Draw()
{
    if (m_currentScreen)
    {
        m_currentScreen->Draw();
    }
}