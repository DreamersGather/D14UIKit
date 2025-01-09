#pragma once

#include "Common/Precompile.h"

#include "Inc/TabGroup.h"

namespace d14uikit
{
    struct TabGroup::Impl
    {
        std::shared_ptr<d14engine::uikit::TabGroup> uiobj = {};

        std::optional<Tab> currSelected = {};

        std::shared_ptr<TabCaption> caption = {};
        std::shared_ptr<Panel> content = {};
    };
}
