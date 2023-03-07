#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit::appearance
{
    struct ViewItem
    {
        enum class State
        {
            Idle, Hover, ActiveSelected, InactiveSelected, ActiveSelectedHover, Count
        };
        struct Appearance : appearance::Appearance
        {
            static void initialize();

            struct Main
            {
                SolidStyle background = {};
                StrokeStyle stroke = {};
            }
            main[(size_t)State::Count] = {};

            struct ThemeStyle
            {
                Main main[(size_t)State::Count] = {};
            };
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
