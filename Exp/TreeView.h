#pragma once

#include "Common/Precompile.h"

#include "Inc/TreeView.h"

namespace d14uikit
{
    struct TreeView::Impl
    {
        std::shared_ptr<d14engine::uikit::TreeView> uiobj = {};

        std::shared_ptr<TreeViewItem> item = {};
        std::shared_ptr<TreeViewItem> rootItem = {};
    };
}
