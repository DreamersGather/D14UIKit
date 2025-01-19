#include "Common/Precompile.h"

#include "ScenePanel.h"

#include "Common.h"
#include "RenderObject.h"

#include "Renderer/Interfaces/DrawLayer.h"
#include "Renderer/Renderer.h"

#include "UIKit/ScenePanel.h"

using namespace d14engine;

namespace d14uikit
{
    ScenePanel::ScenePanel()
        :
        ScenePanel(uikit::makeUIObject<uikit::ScenePanel>()) { }

    _D14_UIKIT_CTOR(ScenePanel)
        :
        Panel(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }

    void ScenePanel::addObject(RenderObject* obj)
    {
        using D3D12Target = renderer::Renderer::CommandLayer::D3D12Target;
        auto& target = std::get<D3D12Target>(pimpl->uiobj->cmdLayer()->drawTarget);
        auto& objset = target.at(pimpl->uiobj->primaryLayer());

        objset.insert(obj->getImpl()->uiobj);
    }

    void ScenePanel::removeObject(RenderObject* obj)
    {
        using D3D12Target = renderer::Renderer::CommandLayer::D3D12Target;
        auto& target = std::get<D3D12Target>(pimpl->uiobj->cmdLayer()->drawTarget);
        auto& objset = target.at(pimpl->uiobj->primaryLayer());

        objset.erase(obj->getImpl()->uiobj);
    }
}
