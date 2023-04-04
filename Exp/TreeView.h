#pragma once

#include "Common/Precompile.h"

#include "Inc/TreeView.h"

namespace d14engine::uikit { struct TreeView; }

namespace d14uikit
{
    struct TreeView::Impl
    {
        std::shared_ptr<d14engine::uikit::TreeView> uiobj = {};
    };
}
