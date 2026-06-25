#pragma once

#include "Domain/Library/BookInfo.hpp"

enum class ScreenCommandType
{
    None,

    OpenBook,

    OpenLibrary,

    OpenSettings,

    OpenUSBMSC,

    ExitApplication
};

struct ScreenCommand
{
    ScreenCommandType Type =
        ScreenCommandType::None;

    BookInfo Book;
};