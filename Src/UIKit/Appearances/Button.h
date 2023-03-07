#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit::appearance
{
    struct Button
    {
        struct Appearance : appearance::Appearance
        {
            Appearance();
            static void initialize();

            SolidStyle foreground = {};
            SolidStyle background = {};
            StrokeStyle stroke = {};

            struct ThemeStyle
            {
                struct Foreground
                {
                    D2D1_COLOR_F color = {};
                }
                foreground = {};
            };
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
