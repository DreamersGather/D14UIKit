#pragma once

#include "Common/Precompile.h"

#include "Inc/MenuSeparator.h"

namespace d14uikit
{
    struct MenuSeparator::Impl
    {
        std::shared_ptr<d14engine::uikit::MenuSeparator> uiobj = {};
    };
}
