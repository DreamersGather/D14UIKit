#pragma once

#include "Common/Precompile.h"

#include "Inc/DraggablePanel.h"

namespace d14uikit
{
    struct DraggablePanel::Impl
    {
        std::shared_ptr<d14engine::uikit::DraggablePanel> uiobj = {};
    };
}
