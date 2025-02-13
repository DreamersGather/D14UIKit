#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/ElevatedButton.h"
#include "UIKit/FilledButton.h"
#include "UIKit/ShadowMask.h"

namespace d14engine::uikit
{
    struct ElevatedButton : appearance::ElevatedButton, FilledButton
    {
        ElevatedButton(
            ShrdPtrParam<IconLabel> content,
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {});

        ElevatedButton(
            WstrParam text = L"Button",
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {});

        ShadowMask shadow = {};

        _D14_SET_APPEARANCE_GETTER(ElevatedButton)

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1LayerHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        void onSizeHelper(SizeEvent& e) override;

        void onChangeThemeHelper(WstrParam themeName) override;
    };
}
