#pragma once

#include <Arduino.h>

class IFile
{
public:
    virtual ~IFile() = default;

    virtual bool IsOpen() const = 0;

    virtual size_t Read(
        void *buffer,
        size_t size) = 0;

    virtual bool Seek(
        uint32_t position) = 0;

    virtual uint32_t Position() const = 0;

    virtual uint32_t Size() = 0;

    virtual void Close() = 0;
};