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
        const std::string &bookPath,
        BookSettings &settings) override;

    bool Save(const std::string &bookPath,
              const BookSettings &settings) override;

private:
    std::string SettingsPath(
        const std::string &bookPath) const;

    IStorage &m_storage;
};