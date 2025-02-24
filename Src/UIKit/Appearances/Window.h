#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnumMagic.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"

namespace d14engine::uikit::appearance
{
    struct Window
    {
        enum class ThreeBrosState
        {
            Idle, Hover, Down, CloseIdle, CloseHover, CloseDown
        };
        struct Appearance : appearance::Appearance
        {
            static void initialize();

            SolidStyle background = {};

            struct Shadow
            {
                D2D1_COLOR_F color = {};

                float standardDeviation = 3.0f;
            }
            shadow = {};

            struct CaptionPanel
            {
                SolidStyle background = {};
            }
            captionPanel = {};

            struct DecorativeBar
            {
                // Call loadDecorativeBarBrush after changing this field.
                struct GradientColor
                {
                    D2D1_COLOR_F _0_0 = {}, _0_5 = {}, _1_0 = {};
                }
                gradientColor = {};
            }
            decorativeBar = {};

            struct ThreeBrothers
            {
                SolidStyle foreground = {};
                SolidStyle background = {};
            }
            threeBrothers[cpp_lang_utils::enumCount<ThreeBrosState>] = {};

            float maskOpacityWhenDragAboveTabGroup = 0.5f;

            struct ThemeData
            {
                struct Background
                {
                    D2D1_COLOR_F color = {};
                }
                background = {};

                struct Shadow
                {
                    D2D1_COLOR_F color = {};
                }
                shadow = {};

                struct CaptionPanel
                {
                    struct Background
                    {
                        D2D1_COLOR_F color = {};
                    }
                    background = {};
                }
                captionPanel = {};

                struct DecorativeBar
                {
                    struct GradientColor
                    {
                        D2D1_COLOR_F _0_0 = {}, _0_5 = {}, _1_0 = {};
                    }
                    gradientColor = {};
                }
                decorativeBar = {};

                struct ThreeBrothers
                {
                    SolidStyle foreground = {};
                    SolidStyle background = {};
                }
                threeBrothers[cpp_lang_utils::enumCount<ThreeBrosState>] = {};
            };
            _D14_SET_THEME_DATA_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
