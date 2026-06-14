#include "Platform/Storage/SDStorage.hpp"
#include "Core/Config.hpp" 
#include "Platform/Storage/SDFile.hpp"

static const String TAG = "SDStorage";

bool SDStorage::Init()
{
    m_spi.begin(
        Config::SPI_CLK,
        Config::SPI_MISO,
        Config::SPI_MOSI,
        Config::SD_CS);

    SdSpiConfig sdConfig(
        Config::SD_CS,
        SHARED_SPI,
        SD_SCK_MHZ(40),
        &m_spi);

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

IFile *SDStorage::Open(
    const String &path)
{
    FsFile file;

    if (!file.open(
            path.c_str(),
            O_RDONLY))
    {
        return nullptr;
    }

    return new SDFile(
        std::move(file));
}
