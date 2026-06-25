#pragma once

#include "Platform/Input/Button.hpp"

class BaseOverlay
{
public:
    virtual ~BaseOverlay() = default;

    virtual void Show()
    {
        m_visible = true;
    }

    virtual void Hide()
    {
        m_visible = false;
    }

    bool IsVisible() const
    {
        return m_visible;
    }

    virtual void Draw() = 0;

    virtual void HandleButton(Button button) = 0;

protected:
    bool m_visible = false;
};