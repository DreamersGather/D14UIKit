#pragma once

#include "Common/Precompile.h"

#include "Inc/TabGroup.h"

namespace d14engine::uikit { struct TabGroup; }

namespace d14uikit
{
    struct TabGroup::Impl
    {
        std::shared_ptr<d14engine::uikit::TabGroup> uiobj = {};
    };
}
