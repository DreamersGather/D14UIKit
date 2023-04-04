#pragma once

#include "Common/Precompile.h"

#include "Inc/MenuItem.h"

namespace d14engine::uikit { struct MenuItem; }

namespace d14uikit
{
    struct MenuItem::Impl
    {
        std::shared_ptr<d14engine::uikit::MenuItem> uiobj = {};

        PopupMenu* associatedMenu = nullptr;

        Font font = Font(L"Default/Normal/16");
    };
}
