#pragma once

#include "Common/Precompile.h"

#include "Inc/RawTextInput.h"

namespace d14uikit
{
    struct RawTextInput::Impl
    {
        std::shared_ptr<d14engine::uikit::RawTextInput> uiobj = {};

        std::shared_ptr<Label> placeholder = {};
    };
}
