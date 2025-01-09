#pragma once

#include "Common/Precompile.h"

#include "Inc/VertSlider.h"

namespace d14uikit
{
    struct VertSlider::Impl
    {
        std::shared_ptr<d14engine::uikit::VertSlider> uiobj = {};

        std::shared_ptr<Label> valueLabel = {};
    };
}
