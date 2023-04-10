#pragma once

#include "Common/Precompile.h"

#include "Inc/Window.h"

namespace d14engine::uikit { struct Window; }

namespace d14uikit
{
    struct Window::Impl
    {
        std::shared_ptr<d14engine::uikit::Window> uiobj = {};

        Image* icon = nullptr;

        Font font = Font(L"Default/Normal/16");

        Panel* content = nullptr;
    };
}
