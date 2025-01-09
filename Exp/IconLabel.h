#pragma once

#include "Common/Precompile.h"

#include "Inc/IconLabel.h"

namespace d14uikit
{
    struct IconLabel::Impl
    {
        std::shared_ptr<d14engine::uikit::IconLabel> uiobj = {};

        Image* icon = nullptr;

        std::shared_ptr<Label> label = {};
    };
}
