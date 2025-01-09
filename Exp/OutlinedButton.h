#pragma once

#include "Common/Precompile.h"

#include "Inc/OutlinedButton.h"

namespace d14uikit
{
    struct OutlinedButton::Impl
    {
        std::shared_ptr<d14engine::uikit::OutlinedButton> uiobj = {};
    };
}
