#pragma once

#include "Common/Precompile.h"

#include "Inc/TabCaption.h"

namespace d14uikit
{
    struct TabCaption::Impl
    {
        std::shared_ptr<d14engine::uikit::TabCaption> uiobj = {};

        std::shared_ptr<IconLabel> title = {};
    };
}
