#pragma once

#include "Common/Precompile.h"

#include "Inc/GridLayout.h"

namespace d14engine::uikit { struct GridLayout; }

namespace d14uikit
{
    struct GridLayout::Impl
    {
        std::shared_ptr<d14engine::uikit::GridLayout> uiobj = {};
    };
}
