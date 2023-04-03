#pragma once

#include "Inc/ComboBox.h"

namespace d14engine::uikit { struct ComboBox; }

namespace d14uikit
{
    struct ComboBox::Impl
    {
        std::shared_ptr<d14engine::uikit::ComboBox> uiobj = {};

        std::shared_ptr<PopupMenu> dropDownMenu = {};
    };
}
