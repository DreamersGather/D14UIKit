#pragma once

#include "Common/Precompile.h"

#include "Inc/Window.h"

namespace d14uikit
{
    struct Window::Impl
    {
        std::shared_ptr<d14engine::uikit::Window> uiobj = {};

        std::shared_ptr<IconLabel> caption = {};

        Panel* content = nullptr;
    };
}
