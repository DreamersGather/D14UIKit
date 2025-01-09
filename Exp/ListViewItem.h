#pragma once

#include "Common/Precompile.h"

#include "Inc/ListViewItem.h"

namespace d14uikit
{
    struct ListViewItem::Impl
    {
        std::shared_ptr<d14engine::uikit::ListViewItem> uiobj = {};
    };
}
