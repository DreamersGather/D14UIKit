﻿#pragma once

#include "Common/Precompile.h"

#include "Inc/FlatButton.h"

namespace d14uikit
{
    struct FlatButton::Impl
    {
        std::shared_ptr<d14engine::uikit::FlatButton> uiobj = {};
    };
}
