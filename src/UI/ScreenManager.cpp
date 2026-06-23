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

ScreenCommand ScreenManager::HandleButton(Button button)
{
    if (m_currentScreen)
    {
        return m_currentScreen->HandleButton(
            button);
    }

    return {};
}

void ScreenManager::Draw()
{
    if (m_currentScreen)
    {
        m_currentScreen->Refresh();
    }
}