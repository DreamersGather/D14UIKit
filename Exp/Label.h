#pragma once

#include "Inc/Label.h"

namespace d14engine::uikit { struct Label; }

namespace d14uikit
{
    struct Label::Impl
    {
        std::shared_ptr<d14engine::uikit::Label> uiobj = {};

        Font font = Font(L"Default/Normal/16");
    };
}
