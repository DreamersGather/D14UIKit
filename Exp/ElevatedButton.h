#pragma once

#include "Common/Precompile.h"

#include "Inc/ElevatedButton.h"

namespace d14engine::uikit { struct ElevatedButton; }

namespace d14uikit
{
    struct ElevatedButton::Impl
    {
        std::shared_ptr<d14engine::uikit::ElevatedButton> uiobj = {};
    };
}
