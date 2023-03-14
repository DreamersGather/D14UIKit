#pragma once

#include "Inc/FlatButton.h"

namespace d14engine::uikit { struct FlatButton; }

namespace d14uikit
{
    struct FlatButton::Impl
    {
        std::shared_ptr<d14engine::uikit::FlatButton> uiobj = {};
    };
}
