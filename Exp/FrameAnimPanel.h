#pragma once

#include "Common/Precompile.h"

#include "Inc/FrameAnimPanel.h"

namespace d14uikit
{
    struct FrameAnimPanel::Impl
    {
        std::shared_ptr<d14engine::uikit::FrameAnimPanel> uiobj = {};

        std::vector<Image*> frames = {};

        float frameTimeSpan = 1.0f;

        float bitmapOpacity = 1.0f;
        BitmapInterpMode bitmapInterpMode = BitmapInterpMode::Linear;
    };
}
