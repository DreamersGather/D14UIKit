#pragma once

#include "Common/Precompile.h"

#include "IDrawObject2D.h"

namespace d14engine::renderer
{
    struct DrawObject2D : IDrawObject2D
    {
    protected:
        bool m_visible = true;

        virtual void onRendererUpdateObject2DHelper(Renderer* rndr);
        virtual void onRendererDrawD2d1ObjectHelper(Renderer* rndr);

    public:
        bool isD2d1ObjectVisible() const override;

        void setD2d1ObjectVisible(bool value) override;

        void onRendererUpdateObject2D(Renderer* rndr) override;

        Function<void(DrawObject2D*, Renderer*)>
            f_onRendererUpdateObject2DBefore = {},
            f_onRendererUpdateObject2DAfter = {};

        void onRendererDrawD2d1Object(Renderer* rndr) override;

        Function<void(DrawObject2D*, Renderer*)>
            f_onRendererDrawD2d1ObjectBefore = {},
            f_onRendererDrawD2d1ObjectAfter = {};
    };
}
