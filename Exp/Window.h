#pragma once

#include "Inc/Window.h"

namespace d14engine::uikit { struct Window; }

namespace d14uikit
{
    struct Window::Impl
    {
        std::shared_ptr<d14engine::uikit::Window> uiobj = {};

        Panel* content = nullptr;
    };
}
