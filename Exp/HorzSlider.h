#pragma once

#include "Common/Precompile.h"

#include "Inc/HorzSlider.h"

namespace d14uikit
{
    struct HorzSlider::Impl
    {
        std::shared_ptr<d14engine::uikit::HorzSlider> uiobj = {};

        std::shared_ptr<Label> valueLabel = {};
    };
}
