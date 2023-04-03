#pragma once

#include "Inc/ViewItem.h"

namespace d14engine::uikit { struct ViewItem; }

namespace d14uikit
{
    struct ViewItem::Impl
    {
        std::shared_ptr<d14engine::uikit::ViewItem> uiobj = {};

        Image* icon = nullptr;
    };
}
