#pragma once

#include <memory>

#include "Interfaces/IFile.hpp"

class IDocument
{
public:
    virtual ~IDocument() = default;

    virtual bool IsOpen() = 0;

    virtual String ReadAll() = 0;
};