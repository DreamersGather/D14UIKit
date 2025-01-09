#pragma once

#include "Inc/LabelArea.h"

namespace d14uikit
{
    struct LabelArea::Impl
    {
        std::shared_ptr<d14engine::uikit::LabelArea> uiobj = {};
    };
}
