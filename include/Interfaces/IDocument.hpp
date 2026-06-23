#pragma once

#include <memory>

#include "Interfaces/IFile.hpp"
#include "Interfaces/FileMode.hpp"

class IDocument
{
public:
    virtual ~IDocument() = default;

    virtual bool IsOpen() = 0;

    virtual String ReadAll() = 0;

    virtual size_t Size() = 0;
};