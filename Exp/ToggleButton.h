#pragma once

#include "Common/Precompile.h"

#include "Inc/ToggleButton.h"

namespace d14uikit
{
    struct ToggleButton::Impl
    {
        std::shared_ptr<d14engine::uikit::ToggleButton> uiobj = {};
    };
}
