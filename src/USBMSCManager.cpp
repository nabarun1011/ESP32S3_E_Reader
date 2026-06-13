#include "USBMSCManager.hpp"
#include <USB.h>

SdFat32 *USBMSCManager::m_sd = nullptr;

static USBMSC msc;

static int32_t onRead(
    uint32_t lba,
    uint32_t offset,
    void *buffer,
    uint32_t bufsize)
{
    static uint32_t count = 0;

    count++;

    if ((count % 100) == 0)
    {
        Serial.printf("Reads=%lu\n", count);
    }

    uint32_t sectors = bufsize / 512;

    if (!USBMSCManager::m_sd->card()->readSectors(
            lba,
            (uint8_t *)buffer,
            sectors))
    {
        return -1;
    }

    return bufsize;
}

static int32_t onWrite(
    uint32_t lba,
    uint32_t offset,
    uint8_t *buffer,
    uint32_t bufsize)
{
    if (lba == 8192)
    {
        uint8_t *b = (uint8_t *)buffer;

        Serial.printf(
            "Boot: %02X %02X %02X %02X\n",
            b[0], b[1], b[2], b[3]);
    }

    uint32_t sectors = bufsize / 512;

    if (!USBMSCManager::m_sd->card()->writeSectors(
            lba,
            buffer,
            sectors))
    {
        return -1;
    }

    return bufsize;
}

static bool onStartStop(
    uint8_t power_condition,
    bool start,
    bool load_eject)
{
    return true;
}

bool USBMSCManager::Init(
    SdFat32 *sd)
{
    Serial.println("USBMSCManager::Init");
    m_sd = sd;
    Serial.printf("Sectors: %lu\n",
                  sd->card()->sectorCount());
    msc.vendorID("ESP32");
    msc.productID("SDCARD");
    msc.productRevision("1.0");

    msc.onRead(onRead);
    msc.onWrite(onWrite);
    msc.onStartStop(onStartStop);

    msc.mediaPresent(true);

    bool ok = msc.begin(
        sd->card()->sectorCount(),
        512);
    Serial.printf("msc.begin() returned %d\n", ok);
    Serial.println("USBMSCManager::Beginning USB");
    USB.begin();
    Serial.println("USBMSCManager::Init done");
    return true;
}