#pragma once

#include "Domain/Settings/BookSettings.hpp"

class IBookSettingsRepository
{
public:
    virtual bool Load(
        const String &bookPath,
        BookSettings &settings) = 0;

    virtual bool Save(
        const String &bookPath,
        const BookSettings &settings) = 0;

    virtual ~IBookSettingsRepository() = default;
};