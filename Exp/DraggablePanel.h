﻿#pragma once

#include "Inc/DraggablePanel.h"

namespace d14engine::uikit { struct DraggablePanel; }

namespace d14uikit
{
    struct DraggablePanel::Impl
    {
        std::shared_ptr<d14engine::uikit::DraggablePanel> uiobj = {};
    };
}
