#pragma once

#include "Interfaces/IScreen.hpp"
#include "Domain/Library/LibraryScanner.hpp"
#include "Interfaces/IDisplay.hpp"

class LibraryScreen : public IScreen
{
public:
    explicit LibraryScreen(
        LibraryScanner &library,
        IDisplay &display);

    void Enter() override;

    void Exit() override;

    void Update() override;

    void Draw() override;

private:
    LibraryScanner &m_library;

    IDisplay &m_display;
};