#include "Common/Precompile.h"

#include "DrawLayer.h"

namespace d14engine::renderer
{
    void DrawLayer::onRendererUpdateLayerHelper(Renderer* rndr)
    {
        // This method intentionally left blank.
    }

    void DrawLayer::onRendererDrawD3d12LayerHelper(Renderer* rndr)
    {
        // This method intentionally left blank.
    }

    bool DrawLayer::isD3d12LayerVisible() const
    {
        return m_visible;
    }

    void DrawLayer::setD3d12LayerVisible(bool value)
    {
        m_visible = value;
    }

    void DrawLayer::onRendererUpdateLayer(Renderer* rndr)
    {
        if (f_onRendererUpdateLayerBefore)
        {
            f_onRendererUpdateLayerBefore(this, rndr);
        }
        onRendererUpdateLayerHelper(rndr);

        if (f_onRendererUpdateLayerAfter)
        {
            f_onRendererUpdateLayerAfter(this, rndr);
        }
    }

    void DrawLayer::onRendererDrawD3d12Layer(Renderer* rndr)
    {
        if (f_onRendererDrawD3d12LayerBefore)
        {
            f_onRendererDrawD3d12LayerBefore(this, rndr);
        }
        onRendererDrawD3d12LayerHelper(rndr);

        if (f_onRendererDrawD3d12LayerAfter)
        {
            f_onRendererDrawD3d12LayerAfter(this, rndr);
        }
    }
}
