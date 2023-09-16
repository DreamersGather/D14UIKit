#pragma once

#include "Common/Precompile.h"

#include "IDrawObject.h"

namespace d14engine::renderer
{
    struct DrawObject : IDrawObject
    {
    protected:
        bool m_visible = true;

        virtual void onRendererUpdateObjectHelper(Renderer* rndr);
        virtual void onRendererDrawD3d12ObjectHelper(Renderer* rndr);

    public:
        bool isD3d12ObjectVisible() const override;

        void setD3d12ObjectVisible(bool value) override;

        void onRendererUpdateObject(Renderer* rndr) override;

        Function<void(DrawObject*, Renderer*)>
            f_onRendererUpdateObjectBefore = {},
            f_onRendererUpdateObjectAfter = {};

        void onRendererDrawD3d12Object(Renderer* rndr) override;

        Function<void(DrawObject*, Renderer*)>
            f_onRendererDrawD3d12ObjectBefore = {},
            f_onRendererDrawD3d12ObjectAfter = {};
    };
}
