#pragma once

#include "Interfaces/IDisplay.hpp"

class DisplayManager : public IDisplay
{
public:
    bool Init() override;

    void Clear() override;
};