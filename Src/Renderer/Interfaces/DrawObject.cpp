#include "Common/Precompile.h"

#include "DrawObject.h"

namespace d14engine::renderer
{
    void DrawObject::onRendererUpdateObjectHelper(Renderer* rndr)
    {
        // This method intentionally left blank.
    }

    void DrawObject::onRendererDrawD3d12ObjectHelper(Renderer* rndr)
    {
        // This method intentionally left blank.
    }

    bool DrawObject::isD3d12ObjectVisible() const
    {
        return m_visible;
    }

    void DrawObject::setD3d12ObjectVisible(bool value)
    {
        m_visible = value;
    }

    void DrawObject::onRendererUpdateObject(Renderer* rndr)
    {
        if (f_onRendererUpdateObjectBefore)
        {
            f_onRendererUpdateObjectBefore(this, rndr);
        }
        onRendererUpdateObjectHelper(rndr);

        if (f_onRendererUpdateObjectAfter)
        {
            f_onRendererUpdateObjectAfter(this, rndr);
        }
    }

    void DrawObject::onRendererDrawD3d12Object(Renderer* rndr)
    {
        if (f_onRendererDrawD3d12ObjectBefore)
        {
            f_onRendererDrawD3d12ObjectBefore(this, rndr);
        }
        onRendererDrawD3d12ObjectHelper(rndr);

        if (f_onRendererDrawD3d12ObjectAfter)
        {
            f_onRendererDrawD3d12ObjectAfter(this, rndr);
        }
    }
}
