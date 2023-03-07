#pragma once

#include "Common/Precompile.h"

#include "Renderer/Interfaces/IDrawObject.h"

namespace d14engine::renderer
{
    struct ICamera : IDrawObject
    {
        using Viewport = D3D12_VIEWPORT;
        using Scissors = D3D12_RECT;

        virtual Viewport viewport() const = 0;

        virtual Scissors scissors() const = 0;

        virtual void onViewResize(UINT viewWidth, UINT viewHeight) = 0;
    };
}
