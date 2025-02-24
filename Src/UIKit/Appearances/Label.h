#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit::appearance
{
    struct Label
    {
        struct Appearance : appearance::Appearance
        {
            Appearance();
            static void initialize();

            SolidStyle foreground = {};
            SolidStyle background = {};
            StrokeStyle stroke = {};

            // i.e. foreground when disabled
            SolidStyle secondaryForeground = {};

            struct ThemeData
            {
                struct Foreground
                {
                    D2D1_COLOR_F color = {};
                }
                foreground = {},
                secondaryForeground = {};
            };
            _D14_SET_THEME_DATA_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
