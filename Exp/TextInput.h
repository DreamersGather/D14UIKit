#pragma once

#include "Common/Precompile.h"

#include "Inc/TextInput.h"

namespace d14uikit
{
    struct TextInput::Impl
    {
        std::shared_ptr<d14engine::uikit::TextInput> uiobj = {};
    };
}
