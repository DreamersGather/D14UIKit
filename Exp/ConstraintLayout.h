#pragma once

#include "Common/Precompile.h"

#include "Inc/ConstraintLayout.h"

namespace d14uikit
{
    struct ConstraintLayout::Impl
    {
        std::shared_ptr<d14engine::uikit::ConstraintLayout> uiobj = {};
    };
}
