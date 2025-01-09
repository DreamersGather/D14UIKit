#pragma once

#include "Common/Precompile.h"

#include "Inc/MenuItem.h"

namespace d14uikit
{
    struct MenuItem::Impl
    {
        std::shared_ptr<d14engine::uikit::MenuItem> uiobj = {};

        std::shared_ptr<Label> hotkey = {};

        PopupMenu* associatedMenu = nullptr;
    };
}
