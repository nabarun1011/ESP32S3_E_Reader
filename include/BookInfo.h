#pragma once
#include <Arduino.h>
#include <FS.h>


//Represents individual book info in library
struct BookInfo
{
    String title;
    String path;
    FS *storage;
};