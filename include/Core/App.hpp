#pragma once

#include "Platform/Storage/SDStorage.hpp"
#include "Platform/Display/EPDDisplay.hpp"
#include "Domain/Library/LibraryScanner.hpp"
#include "UI/ScreenManager.hpp"
#include "UI/Screens/LibraryScreen.hpp"
#include "UI/Screens/ReaderScreen.hpp"
#include "Domain/Reader/TextDocument.hpp"

class App
{
private:
    SDStorage m_storage;

    LibraryScanner m_library;

    EPDDisplay m_display;

    ScreenManager m_screenManager;

    LibraryScreen m_libraryScreen;

    TextDocument m_document;
    ReaderScreen m_readerScreen;

public:
    App();
    bool Init();
    void Update();
};
