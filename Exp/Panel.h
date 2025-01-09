#pragma once

#include "Common/Precompile.h"

#include "Inc/Panel.h"

#include "UIKit/SolidStyle.h"
#include "UIKit/StrokeStyle.h"

namespace d14uikit
{
    struct Panel::Impl
    {
        std::shared_ptr<d14engine::uikit::Panel> uiobj = {};

        bool animState = false;

        Image* image = nullptr;

        BitmapInterpMode bitmapInterpMode = BitmapInterpMode::Linear;

        d14engine::uikit::SolidStyle bkgn = { .opacity = 0.0f };
        d14engine::uikit::StrokeStyle outline = { .opacity = 0.0f };

        Panel* parent = nullptr;
    };
}
