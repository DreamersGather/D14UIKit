#pragma once

#include "Common/Precompile.h"

#include "Inc/Label.h"

namespace d14uikit
{
    struct Label::Impl
    {
        std::shared_ptr<d14engine::uikit::Label> uiobj = {};

        Font* font = nullptr;
    };
}
