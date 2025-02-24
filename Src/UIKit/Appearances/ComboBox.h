#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/FlatButton.h"

namespace d14engine::uikit::appearance
{
    struct ComboBox
    {
        struct Appearance : AppearanceProxy<FlatButton::Appearance>
        {
            static void initialize();

            struct Arrow
            {
                struct Geometry
                {
                    struct Line
                    {
                        D2D1_POINT_2F point0 = {}, point1 = {};
                    }
                    line0 = { { -34.0f, 16.0f }, { -27.0f, 24.0f } },
                    line1 = { { -27.0f, 24.0f }, { -20.0f, 16.0f } };
                }
                geometry = {};

                SolidStyle background = {};
                // i.e. background when disabled
                SolidStyle secondaryBackground = {};

                float strokeWidth = 2.0f;
            }
            arrow = {};

            struct ThemeData : FlatButton::Appearance::ThemeData
            {
                struct Arrow
                {
                    struct Background
                    {
                        D2D1_COLOR_F color = {};
                    }
                    background = {},
                    secondaryBackground = {};
                }
                arrow = {};
            };
            _D14_SET_THEME_DATA_MAP_DECL;

            void changeTheme(FlatButton::Appearance& appearance, WstrParam themeName) override;
        }
        appearance = {};
    };
}
