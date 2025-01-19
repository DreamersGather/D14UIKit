#include "Common/Precompile.h"

#include "RenderObject.h"

#include "Common.h"

using namespace d14engine;

namespace d14uikit
{
    RenderObject::RenderObject()
        :
        RenderObject(std::make_shared<uikit::RenderObject>()) { }

    _D14_UIKIT_CTOR(RenderObject)
        :
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>())
    {
        pimpl->uiobj = uiobj;

        pimpl->uiobj->f_onRendererUpdateObjectAfter = [this]
        (renderer::DrawObject* obj, renderer::Renderer* rndr)
        {
            update();
            if (pcallback->update)
            {
                pcallback->update(this);
            }
        };
        pimpl->uiobj->f_onRendererDrawD3d12ObjectAfter = [this]
        (renderer::DrawObject* obj, renderer::Renderer* rndr)
        {
            draw();
            if (pcallback->draw)
            {
                pcallback->draw(this);
            }
        };
    }

    int RenderObject::priority() const
    {
        return pimpl->uiobj->priority();
    }

    void RenderObject::setPriority(int value)
    {
        pimpl->uiobj->setPriority(value);
    }

    RenderObject::Callback& RenderObject::callback() const { return *pcallback; }

    void RenderObject::update() { }

    void RenderObject::draw() { }
}
