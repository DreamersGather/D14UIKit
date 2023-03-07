#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/ViewItem.h"

namespace d14engine::uikit::appearance
{
    struct MenuItem
    {
        struct Appearance : AppearanceProxy<ViewItem::Appearance>
        {
            static void initialize();

            struct Arrow
            {
                struct Geometry
                {
                    float rightOffset = -30.0f;

                    struct Line
                    {
                        D2D1_POINT_2F point0 = {}, point1 = {};
                    }
                    line0 = { { 12.0f, 8.0f }, { 20.0f, 16.0f } },
                    line1 = { { 20.0f, 15.0f }, { 12.0f, 23.0f } };
                }
                geometry = {};

                SolidStyle background = {};

                float strokeWidth = 1.0f;
            }
            arrow = {};

#pragma warning(push)
#pragma warning(disable : 26495)
            struct ThemeStyle : ViewItem::Appearance::ThemeStyle
            {
                struct Arrow
                {
                    SolidStyle background = {};
                }
                arrow = {};
            };
#pragma warning(pop) // C26495: ViewItem::Appearance::ThemeStyle::main is uninitialized.
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(ViewItem::Appearance& appearance, WstrParam themeName) override;
        }
        appearance = {};
    };
}
