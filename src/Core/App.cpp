#include "Core/App.hpp"

static const String TAG = "App";

bool App::Init()
{
    if (m_storage.Init() == false)
    {
        Serial.printf("%s : Storage Init Failed", TAG);
        return false;
    }

    Serial.printf("%s : Root Exists: %s\n", TAG, m_storage.Exists("/") ? "true" : "false");

    Serial.printf("%s : Root dir: %s\n", TAG, m_storage.IsDirectory("/")? "true" : "false");

    Serial.printf("%s : boot size: %u\n", TAG, m_storage.FileSize("/Test.txt"));

    if (!m_display.Init())
    {
        return false;
    }

    return true;
}

void App::Update()
{
}