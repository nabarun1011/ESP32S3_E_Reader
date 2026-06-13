#pragma once

#include <GxEPD2_BW.h>

enum RefreshType
{
    Refresh_None,
    Refresh_UpperHalf,
    Refresh_LowerHalf,
    Refresh_Full,
    Refresh_Header
};

void RequestRefresh(RefreshType type);

void ProcessRender();
