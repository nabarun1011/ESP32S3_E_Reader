#include "Core/App.hpp"

static const char* TAG = "App";

App::App() : m_storage(), m_library(m_storage), m_display()
{
}

bool App::Init()
{
    if (m_storage.Init() == false)
    {
        Serial.printf("%s : Storage Init Failed", TAG);
        return false;
    }

    m_library.Scan();
    const auto &books =
        m_library.GetBooks();
    Serial.printf("%s : Get books\n", TAG);
    Serial.printf(
        "Books found: %u\n",
        books.size());

    for (const auto &book : books)
    {
        Serial.printf(
            "%s (%s)\n",
            book.title.c_str(),
            book.extension.c_str());
    }

    if (!m_display.Init())
    {
        return false;
    }

    return true;
}

void App::Update()
{
}