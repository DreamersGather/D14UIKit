#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit::appearance
{
    struct ScrollView
    {
        enum class ScrollBarState { Idle, Hover, Down, Count };

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
            scrollBar[(size_t)ScrollBarState::Count] = {};

            struct ThemeStyle
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
                scrollBar[(size_t)ScrollBarState::Count] = {};
            };
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
