#include "Platform/Storage/SDStorage.hpp"
#include "Core/Config.hpp"
#include "Platform/Storage/SDFile.hpp"

static const char *TAG = "SDStorage";

SDStorage::SDStorage()
{
}

bool SDStorage::Init()
{
    // m_spi.begin(
    //     Config::SPI_CLK,
    //     Config::SPI_MISO,
    //     Config::SPI_MOSI,
    //     Config::SD_CS);

    Serial.printf("%s : SD init\n", TAG);
    SdSpiConfig sdConfig(
        Config::SD_CS,
        SHARED_SPI,
        SD_SCK_MHZ(40),
        &SPI);

    if (!m_sd.begin(sdConfig))
    {
        Serial.println("SD init failed");
        return false;
    }
    Serial.printf("%s : FAT type: %u\n", TAG, m_sd.fatType());
    return true;
}

bool SDStorage::Exists(const String &path)
{
    return m_sd.exists(path.c_str());
}

bool SDStorage::IsDirectory(const String &path)
{
    FsFile file;

    if (!file.open(
            path.c_str(),
            O_RDONLY))
    {
        return false;
    }

    bool result =
        file.isDirectory();

    file.close();

    return result;
}

size_t SDStorage::FileSize(const String &path)
{
    FsFile file;

    if (!file.open(
            path.c_str(),
            O_RDONLY))
    {
        return 0;
    }

    size_t size = file.size();

    file.close();

    return size;
}


std::unique_ptr<IFile> SDStorage::Open(
    const String &path,
    FileMode mode)
{
    FsFile file;

    switch (mode)
    {
    case FileMode::Read:
        file = m_sd.open(path.c_str(), O_RDONLY);
        break;

    case FileMode::Write:
        file = m_sd.open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
        break;
    }

    if (!file)
    {
        return nullptr;
    }

    return std::make_unique<SDFile>(std::move(file));
}

std::vector<DirectoryEntry> SDStorage::ListDirectory(
    const String &path)
{
    std::vector<DirectoryEntry> entries;

    FsFile dir;

    if (!dir.open(
            path.c_str(),
            O_RDONLY))
    {
        Serial.printf("%s :Failed to open directory: %s\n", TAG, path.c_str());
        return entries;
    }

    FsFile file;

    while (file.openNext(
        &dir,
        O_RDONLY))
    {
        DirectoryEntry entry;

        char name[128];

        file.getName(
            name,
            sizeof(name));

        entry.name = name;

        if (path == "/")
        {
            entry.path = "/" + entry.name;
        }
        else
        {
            entry.path = path + "/" + entry.name;
        }

        entry.isDirectory = file.isDirectory();

        entry.size = file.size();

        // Serial.printf("%s :Found file: %s\n", TAG, entry.name.c_str());
        entries.push_back(entry);

        file.close();
    }

    dir.close();

    return entries;
}
