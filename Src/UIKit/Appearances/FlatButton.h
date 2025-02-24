#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnumMagic.h"

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
            main[cpp_lang_utils::enumCount<uikit::Button::State>] = {};

            struct ThemeData
            {
                Main main[cpp_lang_utils::enumCount<uikit::Button::State>] = {};
            };
            _D14_SET_THEME_DATA_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
