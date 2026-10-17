#pragma once

#include "Domain/Settings/BookSettings.hpp"

class IBookSettingsRepository
{
public:
    virtual bool Load(
        const std::string &bookPath,
        BookSettings &settings) = 0;

    virtual bool Save(
        const std::string &bookPath,
        const BookSettings &settings) = 0;

    virtual ~IBookSettingsRepository() = default;
};