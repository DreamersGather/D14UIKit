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
        // this function depends on GPU commands, so you need to reset/commit the command list manually
        SharedPtr<DefaultBuffer> load(Renderer* rndr, IWICBitmapSource* source);
    }
}
