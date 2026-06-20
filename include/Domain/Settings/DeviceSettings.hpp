// Global device settings

//     Rotation
//         Button
//         mapping(future)
//             Language(future)

#pragma once

#include <stdint.h>
#include "Domain/Reader/ReaderFont.hpp"




struct DeviceSettings
{
    ReaderFont UIFont = ReaderFont::Sans;
    // Display orientation (0,3 = landscapes, 1,2 = portraits)
    uint8_t Orientation = 3;

    
};
