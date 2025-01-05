#pragma once

#include "Common/Precompile.h"

#include "UIKit/AnimationUtils/BitmapSequence.h"
#include "UIKit/Panel.h"

namespace d14engine::uikit
{
    struct FrameAnimPanel : Panel
    {
        using Panel::Panel;

        animation_utils::BitmapSequence bitmapData = {};

    protected:
        // IDrawObject2D
        void onRendererUpdateObject2DHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;
    };
}
