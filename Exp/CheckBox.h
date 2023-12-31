﻿#pragma once

#include "Common/Precompile.h"

#include "Inc/CheckBox.h"

namespace d14engine::uikit { struct CheckBox; }

namespace d14uikit
{
    struct CheckBox::Impl
    {
        std::shared_ptr<d14engine::uikit::CheckBox> uiobj = {};
    };
}
