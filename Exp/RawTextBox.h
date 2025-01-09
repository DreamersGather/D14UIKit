#pragma once

#include "Common/Precompile.h"

#include "Inc/RawTextBox.h"

namespace d14uikit
{
    struct RawTextBox::Impl
    {
        std::shared_ptr<d14engine::uikit::RawTextBox> uiobj = {};
    };
}
