#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnumMagic.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit::appearance
{
    struct ScrollView
    {
        enum class ScrollBarState { Idle, Hover, Down };

        struct Appearance : appearance::Appearance
        {
            Appearance();
            static void initialize();

            SolidStyle background = {};
            StrokeStyle stroke = {};

            struct ScrollBar
            {
                struct Geometry
                {
                    float width = {};
                    float roundRadius = {};
                    float offset = {};
                }
                geometry = {};

                SolidStyle background = {};
            }
            scrollBar[cpp_lang_utils::enumCount<ScrollBarState>] = {};

            struct ThemeData
            {
                struct Background
                {
                    D2D1_COLOR_F color = {};
                }
                background = {};

                struct Stroke
                {
                    D2D1_COLOR_F color = {};
                }
                stroke = {};

                struct ScrollBar
                {
                    SolidStyle background = {};
                }
                scrollBar[cpp_lang_utils::enumCount<ScrollBarState>] = {};
            };
            _D14_SET_THEME_DATA_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
