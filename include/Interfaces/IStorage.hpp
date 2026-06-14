#pragma once

#include <Arduino.h>
#include "Interfaces/IFile.hpp"

class IStorage
{
public:
    virtual ~IStorage() = default;

    virtual bool Init() = 0;

    virtual bool Exists(const String &path) = 0;

    virtual bool IsDirectory(const String &path) = 0;

    virtual size_t FileSize(const String &path) = 0;

    virtual IFile *Open(const String &path) = 0;
};