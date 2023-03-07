#pragma once

#include "Common/Precompile.h"

namespace d14engine
{
    struct ResourcePack
    {
        void * data = {};
        size_t size = {};
    };
    ResourcePack loadResource(WstrParam name, WstrParam type);
}
