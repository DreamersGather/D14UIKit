#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Label.h"

namespace d14engine::uikit::appearance
{
    struct RawTextInput
    {
        struct Appearance : AppearanceProxy<Label::Appearance>
        {
            static void initialize();

            struct BottomLine
            {
                float bottomOffset = -0.5f;

                SolidStyle background = {};

                float strokeWidth = 1.0f;
            }
            bottomLine = {};

            struct ThemeStyle
            {
                SolidStyle background = {};
                StrokeStyle stroke = {};

                struct BottomLine
                {
                    SolidStyle background = {};
                }
                bottomLine = {};
            };
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(Label::Appearance& appearance, WstrParam themeName) override;
        }
        appearance = {};
    };
}
