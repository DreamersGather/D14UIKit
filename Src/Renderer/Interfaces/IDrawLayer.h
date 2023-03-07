#pragma once

#include "Common/Precompile.h"

#include "Common/Interfaces/ISortable.h"

namespace d14engine::renderer
{
    struct Renderer;

    struct IDrawLayer : ISortable<IDrawLayer>
    {
        virtual bool isD3d12LayerVisible() const = 0;

        virtual void setD3d12LayerVisible(bool value) = 0;

        virtual void onRendererUpdateLayer(Renderer* rndr) = 0;

        virtual void onRendererDrawD3d12Layer(Renderer* rndr) = 0;
    };
}
