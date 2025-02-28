#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/OutlinedButton.h"
#include "UIKit/FlatButton.h"

namespace d14engine::uikit
{
    struct OutlinedButton : appearance::OutlinedButton, FlatButton
    {
        OutlinedButton(
            ShrdPtrParam<IconLabel> content,
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {});

        OutlinedButton(
            WstrParam text = L"Button",
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {});

        _D14_SET_APPEARANCE_GETTER(OutlinedButton)

    protected:
        // Panel
        void onChangeThemeStyleHelper(const ThemeStyle& style) override;
    };
}
