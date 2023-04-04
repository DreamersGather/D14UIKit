#pragma once

#include "Common/Precompile.h"

#include "Inc/Panel.h"

#include "UIKit/SolidStyle.h"

namespace d14engine::uikit { struct Panel; }

namespace d14uikit
{
    struct Panel::Impl
    {
        std::shared_ptr<d14engine::uikit::Panel> uiobj = {};

        Image* image = nullptr;

        d14engine::uikit::SolidStyle bkgn = { .opacity = 0.0f };

        Panel* parent = nullptr;
    };
}
