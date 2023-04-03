#pragma once

#include "Inc/ListView.h"

namespace d14engine::uikit { struct ListView; }

namespace d14uikit
{
    struct ListView::Impl
    {
        std::shared_ptr<d14engine::uikit::ListView> uiobj = {};
    };
}
