#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/ViewItem.h"

namespace d14engine::uikit::appearance
{
    struct MenuSeparator
    {
        struct Appearance : AppearanceProxy<ViewItem::Appearance>
        {
            static void initialize();

            SolidStyle background = {};

            float strokeWidth = 1.0f;

            struct ThemeStyle
            {
                SolidStyle background = {};
            };
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(ViewItem::Appearance& appearance, WstrParam themeName) override;
        }
        appearance = {};
    };
}
