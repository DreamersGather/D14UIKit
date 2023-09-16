#pragma once

#include "Common/Precompile.h"

#include "IDrawLayer.h"

namespace d14engine::renderer
{
    struct DrawLayer : IDrawLayer
    {
    protected:
        bool m_visible = true;

        virtual void onRendererUpdateLayerHelper(Renderer* rndr);
        virtual void onRendererDrawD3d12LayerHelper(Renderer* rndr);

    public:
        bool isD3d12LayerVisible() const override;

        void setD3d12LayerVisible(bool value) override;

        void onRendererUpdateLayer(Renderer* rndr) override;

        Function<void(DrawLayer*, Renderer*)>
            f_onRendererUpdateLayerBefore = {},
            f_onRendererUpdateLayerAfter = {};

        void onRendererDrawD3d12Layer(Renderer* rndr) override;

        Function<void(DrawLayer*, Renderer*)>
            f_onRendererDrawD3d12LayerBefore = {},
            f_onRendererDrawD3d12LayerAfter = {};
    };
}
