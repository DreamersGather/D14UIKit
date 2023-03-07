#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/MenuSeparator.h"
#include "UIKit/MenuItem.h"

namespace d14engine::uikit
{
    struct MenuSeparator : appearance::MenuSeparator, MenuItem
    {
        explicit MenuSeparator(const D2D1_RECT_F& rect = {});

        _D14_SET_APPEARANCE_GETTER(MenuSeparator)

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        void onChangeThemeHelper(WstrParam themeName) override;
    };
}
