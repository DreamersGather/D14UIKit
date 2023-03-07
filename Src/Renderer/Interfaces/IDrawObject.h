#pragma once

#include "Common/Precompile.h"

#include "Common/Interfaces/ISortable.h"

namespace d14engine::renderer
{
    struct Renderer;

    struct IDrawObject : ISortable<IDrawObject>
    {
        virtual bool isD3d12ObjectVisible() const = 0;

        virtual void setD3d12ObjectVisible(bool value) = 0;

        virtual void onRendererUpdateObject(Renderer* rndr) = 0;

        virtual void onRendererDrawD3d12Object(Renderer* rndr) = 0;
    };
}
