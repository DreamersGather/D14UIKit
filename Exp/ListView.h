#pragma once

#include "Common/Precompile.h"

#include "Inc/ListView.h"

namespace d14uikit
{
    struct ListView::Impl
    {
        std::shared_ptr<d14engine::uikit::ListView> uiobj = {};

        std::shared_ptr<ListViewItem> item = {};
    };
}
