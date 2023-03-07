#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"

namespace d14engine::uikit::appearance
{
    struct PopupMenu
    {
        struct Appearance : appearance::Appearance
        {
            static void initialize();

            struct Geometry
            {
                // Call loadShadowBitmap after changing this field.
                float extension = 10.0f;

                float roundRadius = 10.0f;
            }
            geometry = {};

            SolidStyle background = {};

            struct Shadow
            {
                // Call loadShadowBitmap after changing this field.
                D2D1_RECT_F offset = { 2.0f, 4.0f, -2.0f, 0.0f };

                D2D1_COLOR_F color = {};

                float standardDeviation = 5.0f;
            }
            shadow = {};

            struct ThemeStyle
            {
                SolidStyle background = {};

                struct Shadow
                {
                    D2D1_COLOR_F color = {};
                }
                shadow = {};
            };
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
