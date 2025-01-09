#pragma once

#include "Common/Precompile.h"

#include "Inc/ViewItem.h"

namespace d14uikit
{
    struct ViewItem::Impl
    {
        std::shared_ptr<d14engine::uikit::ViewItem> uiobj = {};

        std::shared_ptr<IconLabel> content = {};
    };
}
