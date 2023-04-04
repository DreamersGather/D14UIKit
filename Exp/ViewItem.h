#pragma once

#include "Common/Precompile.h"

#include "Inc/ViewItem.h"

namespace d14engine::uikit { struct ViewItem; }

namespace d14uikit
{
    struct ViewItem::Impl
    {
        std::shared_ptr<d14engine::uikit::ViewItem> uiobj = {};

        Image* icon = nullptr;

        Font font = Font(L"Default/Normal/16");
    };
}
