#pragma once

#include "Common/Precompile.h"

#include "Inc/Button.h"

namespace d14engine::uikit { struct Button; }

namespace d14uikit
{
    struct Button::Impl
    {
        std::shared_ptr<d14engine::uikit::Button> uiobj = {};

        Image* icon = nullptr;

        Font font = Font(L"Default/Normal/16");
    };
}
