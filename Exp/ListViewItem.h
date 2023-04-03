#pragma once

#include "Inc/ListViewItem.h"

namespace d14engine::uikit { struct ListViewItem; }

namespace d14uikit
{
    struct ListViewItem::Impl
    {
        std::shared_ptr<d14engine::uikit::ListViewItem> uiobj = {};
    };
}
