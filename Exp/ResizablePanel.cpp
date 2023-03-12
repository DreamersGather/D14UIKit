#include "Common/Precompile.h"

#include "Panel.h"
#include "ResizablePanel.h"

#include "UIKit/ResizablePanel.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    ResizablePanel::ResizablePanel() : ResizablePanel(Passkey{})
    {
        Panel::pimpl->uiobj =
        ResizablePanel::pimpl->uiobj =
        uikit::makeUIObject<uikit::ResizablePanel>(
            D2D1_RECT_F{},
            uikit::resource_utils::g_solidColorBrush);

        Panel::initialize();
        ResizablePanel::initialize();
    }

    bool ResizablePanel::resizable() const
    {
        return pimpl->resizable;
    }

    void ResizablePanel::setResizable(bool value)
    {
        pimpl->resizable = value;

        pimpl->uiobj->isLeftResizable = pimpl->uiobj->isRightResizable =
        pimpl->uiobj->isTopResizable = pimpl->uiobj->isBottomResizable = value;
    }

    bool ResizablePanel::dynamicSizing() const
    {
        return pimpl->uiobj->enableDynamicSizing;
    }

    void ResizablePanel::setDynamicSizing(bool value)
    {
        pimpl->uiobj->enableDynamicSizing = value;
    }

    ResizablePanel::Callback&
        ResizablePanel::callback()
        const { return *pcallback; }

    ResizablePanel::ResizablePanel(Passkey)
        :
        Panel(Panel::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void ResizablePanel::initialize()
    {
        pimpl->uiobj->f_onStartResizing = [this]
        (uikit::ResizablePanel* rsp)
        {
            onStartResizing();
            if (pcallback->onStartResizing)
            {
                pcallback->onStartResizing(this);
            }
        };
        pimpl->uiobj->f_onEndResizing = [this]
        (uikit::ResizablePanel* rsp)
        {
            onEndResizing();
            if (pcallback->onEndResizing)
            {
                pcallback->onEndResizing(this);
            }
        };
    }

    void ResizablePanel::onStartResizing() { }

    void ResizablePanel::onEndResizing() { }
}
