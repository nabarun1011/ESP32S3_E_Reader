#pragma once

#include "Interfaces/IBookSettingsRepository.hpp"
#include "Interfaces/IStorage.hpp"

class BookSettingsRepository
    : public IBookSettingsRepository
{
public:
    explicit BookSettingsRepository(
        IStorage &storage);

    bool Load(
        const String &bookPath,
        BookSettings &settings) override;

    bool Save(
        const String &bookPath,
        const BookSettings &settings) override;

private:
    String SettingsPath(
        const String &bookPath) const;

    IStorage &m_storage;
};