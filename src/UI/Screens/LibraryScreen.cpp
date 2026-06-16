#include "UI/Screens/LibraryScreen.hpp"

const char* TAG = "LibraryScreen";

LibraryScreen::LibraryScreen(
    LibraryScanner &library, IDisplay &display)
    : m_library(library),
      m_display(display)
{
}

void LibraryScreen::Enter()
{
    Serial.printf("%s : LibraryScreen Enter\n", TAG);
    m_library.Scan();
}

void LibraryScreen::Exit()
{
    Serial.printf("%s : LibraryScreen Exit\n", TAG);
}

void LibraryScreen::Update()
{
}

void LibraryScreen::Draw()
{
    m_display.Render([this]()
                     {
                        
                         int y = 20;

                         const auto &books =
                             m_library.GetBooks();

                         for (const auto &book : books)
                         {
                             m_display.DrawText(
                                 10,
                                 y,
                                 book.title);

                             y += 20;
                         } });

    
}