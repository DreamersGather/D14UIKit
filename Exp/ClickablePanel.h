#pragma once

#include "Common/Precompile.h"

#include "Inc/ClickablePanel.h"

namespace d14engine::uikit { struct ClickablePanel; }

namespace d14uikit
{
    struct ClickablePanel::Impl
    {
        std::shared_ptr<d14engine::uikit::ClickablePanel> uiobj = {};
    };
}
