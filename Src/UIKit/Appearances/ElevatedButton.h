#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/Button.h"

namespace d14engine::uikit::appearance
{
    struct ElevatedButton
    {
        struct Appearance : appearance::Appearance
        {
            static void initialize();

            struct Shadow
            {
                // Call loadShadowBitmap after changing this field.
                D2D1_RECT_F offset = { 2.0f, 3.0f, -2.0f, 0.0f };

                D2D1_COLOR_F color[(size_t)uikit::Button::State::Count] = {};

                float standardDeviation = 3.0f;
            }
            shadow = {};

            struct ThemeStyle
            {
                struct Shadow
                {
                    D2D1_COLOR_F color[(size_t)uikit::Button::State::Count] = {};
                }
                shadow = {};
            };
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
