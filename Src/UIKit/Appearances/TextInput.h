#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnumMagic.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit::appearance
{
    struct TextInput
    {
        enum class State { Idle, Hover, Active, Disabled };

        struct Appearance : appearance::Appearance
        {
            static void initialize();

            struct Main
            {
                SolidStyle background = {};
                StrokeStyle stroke = {};
            }
            main[cpp_lang_utils::enumCount<State>] = {};

            struct BottomLine
            {
                float bottomOffset = -1.0f;

                struct Background
                {
                    SolidStyle Static[cpp_lang_utils::enumCount<State>] = {};
                    SolidStyle Dynamic = {};
                }
                background = {};

                float strokeWidth = 2.0f;

                struct Animation
                {
                    bool enabled = true;

                    struct DurationInSecs
                    {
                        float uniform = 0.1f;
                        float variable = 0.2f;
                    }
                    durationInSecs = {};
                }
                animation = {};
            }
            bottomLine = {};

            struct ThemeData
            {
                Main main[cpp_lang_utils::enumCount<State>] = {};

                struct BottomLine
                {
                    struct Background
                    {
                        SolidStyle Static[cpp_lang_utils::enumCount<State>] = {};
                        SolidStyle Dynamic = {};
                    }
                    background = {};
                }
                bottomLine = {};
            };
            _D14_SET_THEME_DATA_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
