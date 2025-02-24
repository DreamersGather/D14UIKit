#pragma once

#include "Window.h"

_D14_UIKIT_FWDEF(MainWindow)

namespace d14uikit
{
    class DllExport MainWindow : public Window
    {
        _D14_UIKIT_PIMPL(MainWindow)

        explicit MainWindow(const std::wstring& title = L"Untitled");

        /////////////////////////
        // Immersive Dark Mode //
        /////////////////////////
        //----------------------------------------------------------------
        // This setting is supported starting with Windows 11 Build 22000!
        //----------------------------------------------------------------

        bool immersiveDarkMode() const;
        // Returns whether the setting was successful.
        bool setImmersiveDarkMode(bool value);

        //////////////////
        // Corner State //
        //////////////////
        //----------------------------------------------------------------
        // This setting is supported starting with Windows 11 Build 22000!
        //----------------------------------------------------------------

        enum class CornerState
        {
            Round = 2,
            RoundSmall = 3,
            DoNotRound = 1
        };
#define SET_CORNER_STATE(Name) constexpr static auto Name = CornerState::Name;

        SET_CORNER_STATE(Round)
        SET_CORNER_STATE(RoundSmall)
        SET_CORNER_STATE(DoNotRound)

#undef SET_CORNER_STATE

        CornerState cornerState() const;
        // Returns whether the setting was successful.
        bool setCornerState(CornerState state);

        //////////////////
        // Border Color //
        //////////////////
        //----------------------------------------------------------------
        // This setting is supported starting with Windows 11 Build 22000!
        //----------------------------------------------------------------

        // TODO: Implement border color property.
    };
}
