#pragma once

#include "Common/Precompile.h"

#include "Inc/Renderer.h"

namespace d14engine
{
    namespace renderer
    {
        struct Renderer;
    }
    namespace uikit
    {
        struct Renderer
        {
            Renderer(renderer::Renderer* rndr) : rndr(rndr) {}

            renderer::Renderer* rndr = nullptr;
        };
    }
}
namespace d14uikit
{
    struct Renderer::Impl
    {
        std::shared_ptr<d14engine::uikit::Renderer> uiobj = {};
    };
}
