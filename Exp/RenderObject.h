#pragma once

#include "Common/Precompile.h"

#include "Inc/RenderObject.h"

#include "Renderer/Interfaces/DrawObject.h"

namespace d14engine
{
    namespace uikit
    {
        struct RenderObject : renderer::DrawObject {};
    }
}
namespace d14uikit
{
    struct RenderObject::Impl
    {
        std::shared_ptr<d14engine::uikit::RenderObject> uiobj = {};
    };
}
