// Global device settings

//     Rotation
//         Button
//         mapping(future)
//             Language(future)

#pragma once

#include <stdint.h>

struct DeviceSettings
{
    // Display orientation (0,3 = landscapes, 1,2 = portraits)
    uint8_t Orientation = 0;
};