#include "UI/Screens/ReaderScreen.hpp"

ReaderScreen::ReaderScreen(
    TextDocument &document,
    IDisplay &display)
    : m_document(document),
      m_display(display),
      m_renderer(display)
{
}

void ReaderScreen::Enter()
{
}

void ReaderScreen::Exit()
{
}

void ReaderScreen::Update()
{
}

void ReaderScreen::Draw()
{
    char buffer[512];

    size_t read =
        m_document.Read(buffer, sizeof(buffer) - 1);

    buffer[read] = '\0';

    m_display.Render(
        [this, buffer]()
        {
            m_renderer.DrawTextBlock(10, 20, String(buffer));
        });
}