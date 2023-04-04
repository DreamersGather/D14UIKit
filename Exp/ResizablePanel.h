#pragma once

#include "Common/Precompile.h"

#include "Inc/ResizablePanel.h"

namespace d14engine::uikit { struct ResizablePanel; }

namespace d14uikit
{
    struct ResizablePanel::Impl
    {
        std::shared_ptr<d14engine::uikit::ResizablePanel> uiobj = {};

        bool resizable = true;
    };
}
