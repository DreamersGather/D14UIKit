#pragma once

#include "Common/Precompile.h"

#include "Renderer/FrameData/FrameSequence.h"

#include "UIKit/BitmapObject.h"

namespace d14engine::uikit::animation_utils
{
    struct BitmapSequence : renderer::FrameSequence<BitmapObject>
    {
        void draw(renderer::Renderer* rndr, const D2D1_RECT_F& rect);
    };
}
