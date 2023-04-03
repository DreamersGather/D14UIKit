#pragma once

#include "Inc/PopupMenu.h"

namespace d14engine::uikit { struct PopupMenu; }

namespace d14uikit
{
    struct PopupMenu::Impl
    {
        std::shared_ptr<d14engine::uikit::PopupMenu> uiobj = {};
    };
}
