#pragma once

#include "Common/Precompile.h"

#include "Inc/Button.h"

namespace d14uikit
{
    struct Button::Impl
    {
        std::shared_ptr<d14engine::uikit::Button> uiobj = {};

        std::shared_ptr<IconLabel> content = {};
    };
}
