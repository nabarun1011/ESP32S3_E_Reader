#pragma once

#include "Interfaces/IDisplay.hpp"
#include "Domain/Reader/TextDocument.hpp"
#include "Interfaces/IScreen.hpp"
#include "Domain/Reader/TextRenderer.hpp"

class ReaderScreen : public IScreen
{
public:
    ReaderScreen(
        TextDocument &document,
        IDisplay &display);

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Draw() override;

private:
    TextDocument &m_document;
    IDisplay &m_display;
    TextRenderer m_renderer;
};