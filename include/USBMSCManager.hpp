#pragma once

#include <USBMSC.h>
#include <SdFat.h>

class USBMSCManager
{
public:
    bool Init(SdFat32 *sd);
    static SdFat32 *m_sd;

private:
    
};