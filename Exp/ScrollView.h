#pragma once

#include "Common/Precompile.h"

#include "Inc/ScrollView.h"

namespace d14uikit
{
    struct ScrollView::Impl
    {
        std::shared_ptr<d14engine::uikit::ScrollView> uiobj = {};

        Panel* content = nullptr;
    };
}
