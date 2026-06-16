#include "UI/ScreenManager.hpp"

void ScreenManager::SetScreen(
    IScreen *screen)
{
    if (m_current)
    {
        m_current->Exit();
    }

    m_current = screen;

    if (m_current)
    {
        m_current->Enter();
    }
}


void ScreenManager::Update()
{
    if (m_current)
    {
        m_current->Update();
    }
}

void ScreenManager::Draw()
{
    if (m_current)
    {
        m_current->Draw();
    }
}