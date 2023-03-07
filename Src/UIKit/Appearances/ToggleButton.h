#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/FlatButton.h"

namespace d14engine::uikit::appearance
{
    struct ToggleButton
    {
        struct Appearance : FlatButton::Appearance
        {
            static void initialize();

            static ThemeStyleMap g_themeStyles;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
