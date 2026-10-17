// Global device settings

//     Rotation
//         Button
//         mapping(future)
//             Language(future)

#pragma once

#include <stdint.h>
#include "Domain/Reader/ReaderFont.hpp"

enum AutoSLeep
{
    Never,
    Minute1,
    Minute5,
    Minute10,
    Minute30
};

struct DeviceSettings
{
    ReaderFont UIFont = ReaderFont::Sans;
    // Display orientation (0,3 = landscapes, 1,2 = portraits)
    uint8_t Orientation = 3;

    AutoSLeep AutoSleep = AutoSLeep::Never;

    bool WifiEnabled = false;
};
