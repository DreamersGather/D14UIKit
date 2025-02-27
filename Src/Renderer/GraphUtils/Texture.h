#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer
{
    struct DefaultBuffer;
    struct Renderer;
}
namespace d14engine::renderer::graph_utils
{
    namespace texture
    {
        // GPU Commands Required
        SharedPtr<DefaultBuffer> load(Renderer* rndr, IWICBitmapSource* source);
    }
}
