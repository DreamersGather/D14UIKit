#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/FlatButton.h"

namespace d14engine::uikit::appearance
{
    struct FilledButton
    {
        struct Appearance : AppearanceProxy<FlatButton::Appearance>
        {
            static void initialize();

            _D14_REF_THEME_DATA_MAP_DECL(FlatButton)

            void changeTheme(FlatButton::Appearance& appearance, WstrParam themeName) override;
        }
        appearance = {};
    };
}
