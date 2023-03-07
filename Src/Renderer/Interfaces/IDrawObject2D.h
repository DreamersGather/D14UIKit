#pragma once

#include "Common/Precompile.h"

#include "Common/Interfaces/ISortable.h"

namespace d14engine::renderer
{
    struct Renderer;

    struct IDrawObject2D : ISortable<IDrawObject2D>
    {
        virtual bool isD2d1ObjectVisible() const = 0;

        virtual void setD2d1ObjectVisible(bool value) = 0;

        virtual void onRendererUpdateObject2D(Renderer* rndr) = 0;

        virtual void onRendererDrawD2d1Layer(Renderer* rndr) = 0;

        virtual void onRendererDrawD2d1Object(Renderer* rndr) = 0;
    };
}
