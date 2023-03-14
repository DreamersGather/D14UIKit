﻿#pragma once

#include "Inc/OutlinedButton.h"

namespace d14engine::uikit { struct OutlinedButton; }

namespace d14uikit
{
    struct OutlinedButton::Impl
    {
        std::shared_ptr<d14engine::uikit::OutlinedButton> uiobj = {};
    };
}
