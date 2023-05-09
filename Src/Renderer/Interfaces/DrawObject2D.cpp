#include "Common/Precompile.h"

#include "DrawObject2D.h"

namespace d14engine::renderer
{
    void DrawObject2D::onRendererUpdateObject2DHelper(Renderer* rndr)
    {
        // This method intentionally left blank.
    }

    void DrawObject2D::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        // This method intentionally left blank.
    }

    bool DrawObject2D::isD2d1ObjectVisible() const
    {
        return m_visible;
    }

    void DrawObject2D::setD2d1ObjectVisible(bool value)
    {
        m_visible = value;
    }

    void DrawObject2D::onRendererUpdateObject2D(Renderer* rndr)
    {
        if (f_onRendererUpdateObject2DBefore)
        {
            f_onRendererUpdateObject2DBefore(this, rndr);
        }
        onRendererUpdateObject2DHelper(rndr);

        if (f_onRendererUpdateObject2DAfter)
        {
            f_onRendererUpdateObject2DAfter(this, rndr);
        }
    }

    void DrawObject2D::onRendererDrawD2d1Object(Renderer* rndr)
    {
        if (f_onRendererDrawD2d1ObjectBefore)
        {
            f_onRendererDrawD2d1ObjectBefore(this, rndr);
        }
        onRendererDrawD2d1ObjectHelper(rndr);

        if (f_onRendererDrawD2d1ObjectAfter)
        {
            f_onRendererDrawD2d1ObjectAfter(this, rndr);
        }
    }
}
