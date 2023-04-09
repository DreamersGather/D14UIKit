#pragma once

#include "Common/Precompile.h"

#include "Inc/TabCaption.h"

namespace d14engine::uikit { struct TabCaption; }

namespace d14uikit
{
    struct TabCaption::Impl
    {
        std::shared_ptr<d14engine::uikit::TabCaption> uiobj = {};

        Image* icon = nullptr;

        Font font = Font(L"Default/Normal/16");
    };
}
