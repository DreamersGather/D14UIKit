#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/FilledButton.h"
#include "UIKit/FlatButton.h"

namespace d14engine::uikit
{
    struct FilledButton : appearance::FilledButton, FlatButton
    {
        FilledButton(
            ShrdPtrParam<IconLabel> content,
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {});

        FilledButton(
            WstrParam text = L"Button",
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {});

        _D14_SET_APPEARANCE_GETTER(FilledButton)

    protected:
        // Panel
        void onChangeThemeHelper(WstrParam themeName) override;
    };
}
