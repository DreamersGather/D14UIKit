#pragma once

#include "Inc/Button.h"

namespace d14engine::uikit { struct Button; }

namespace d14uikit
{
    struct Button::Impl
    {
        std::shared_ptr<d14engine::uikit::Button> uiobj = {};

        Image* icon = nullptr;
    };
}
