#pragma once

#include "Common/Precompile.h"

#include "Inc/OnOffSwitch.h"

namespace d14engine::uikit { struct OnOffSwitch; }

namespace d14uikit
{
    struct OnOffSwitch::Impl
    {
        std::shared_ptr<d14engine::uikit::OnOffSwitch> uiobj = {};
    };
}
