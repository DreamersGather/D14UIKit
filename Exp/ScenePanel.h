#pragma once

#include "Common/Precompile.h"

#include "Inc/ScenePanel.h"

namespace d14uikit
{
    struct ScenePanel::Impl
    {
        std::shared_ptr<d14engine::uikit::ScenePanel> uiobj = {};
    };
}
