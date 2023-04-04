#pragma once

#include "Common/Precompile.h"

#include "Inc/FilledButton.h"

namespace d14engine::uikit { struct FilledButton; }

namespace d14uikit
{
    struct FilledButton::Impl
    {
        std::shared_ptr<d14engine::uikit::FilledButton> uiobj = {};
    };
}
