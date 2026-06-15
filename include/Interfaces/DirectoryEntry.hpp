#pragma once

#include <Arduino.h>

struct DirectoryEntry
{
    String name;

    String path;

    bool isDirectory;

    uint32_t size;
};