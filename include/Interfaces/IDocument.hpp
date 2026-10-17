#pragma once

#include <memory>

#include "Interfaces/IFile.hpp"
#include "Interfaces/FileMode.hpp"

class IDocument
{
public:
    virtual ~IDocument() = default;

    virtual bool IsOpen() = 0;

    virtual std::string ReadAll() = 0;

    virtual size_t Size() = 0;
};