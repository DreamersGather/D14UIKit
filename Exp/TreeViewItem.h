#pragma once

#include "Common/Precompile.h"

#include "Inc/TreeViewItem.h"

namespace d14engine::uikit { struct TreeViewItem; }

namespace d14uikit
{
    struct TreeViewItem::Impl
    {
        std::shared_ptr<d14engine::uikit::TreeViewItem> uiobj = {};

        State state = Unfolded;
    };
}
