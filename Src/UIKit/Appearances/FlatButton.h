#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/Button.h"

namespace d14engine::uikit::appearance
{
    struct FlatButton
    {
        struct Appearance : appearance::Appearance
        {
            static void initialize();

            struct Main
            {
                SolidStyle foreground = {};
                SolidStyle background = {};
                StrokeStyle stroke = {};
            }
            main[(size_t)uikit::Button::State::Count] = {};

            struct ThemeStyle
            {
                Main main[(size_t)uikit::Button::State::Count] = {};
            };
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
