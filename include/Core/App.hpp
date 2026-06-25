#pragma once

#include "Platform/Storage/SDStorage.hpp"
#include "Platform/Display/EPDDisplay.hpp"
#include "UI/ScreenManager.hpp"
#include "UI/Screens/ReaderScreen.hpp"
#include "Domain/Reader/TextDocument.hpp"
#include "Domain/Settings/DeviceSettings.hpp"
#include "Platform/Input/InputManager.hpp"
#include "Platform/Storage/BookSettingsRepository.hpp"
#include "UI/Screens/LibraryScreen.hpp"

class App
{
private:
    SDStorage m_storage;

    BookSettingsRepository m_bookSettingsRepository;

    EPDDisplay m_display;

    DeviceSettings m_deviceSettings;

    LibrarySettings m_libSettings;

    ScreenManager m_screenManager;

    LibraryScreen m_libraryScreen;

    TextDocument m_document;    

    ReaderScreen m_readerScreen;

    InputManager m_inputManager;

public:
    App();
    bool Init();
    void Update();
};
