#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnumMagic.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit::appearance
{
    struct ViewItem
    {
        enum class State
        {
            Idle, Hover, ActiveSelected, InactiveSelected, ActiveSelectedHover
        };
        struct Appearance : appearance::Appearance
        {
            static void initialize();

            struct Main
            {
                SolidStyle background = {};
                StrokeStyle stroke = {};
            }
            main[cpp_lang_utils::enumCount<State>] = {};

            struct ThemeData
            {
                Main main[cpp_lang_utils::enumCount<State>] = {};
            };
            _D14_SET_THEME_DATA_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
