#pragma once
#include <Arduino.h>


// Represents individual book info in library
struct BookInfo
{
    String title;
    String path;
    String extension;
};