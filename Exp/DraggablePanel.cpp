#include "Common/Precompile.h"

#include "DraggablePanel.h"
#include "Panel.h"

#include "UIKit/DraggablePanel.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    DraggablePanel::DraggablePanel() : DraggablePanel(Passkey{})
    {
        Panel::pimpl->uiobj =
        DraggablePanel::pimpl->uiobj =
        uikit::makeUIObject<uikit::DraggablePanel>(
            D2D1_RECT_F{},
            uikit::resource_utils::g_solidColorBrush);

        Panel::initialize();
        DraggablePanel::initialize();
    }

    DraggablePanel::DraggablePanel(Passkey)
        :
        Panel(Panel::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void DraggablePanel::initialize()
    {
        pimpl->uiobj->f_onStartDragging = [this]
        (uikit::DraggablePanel* dgp)
        {
            onStartDragging();
            if (pcallback->onStartDragging)
            {
                pcallback->onStartDragging(this);
            }
        };
        pimpl->uiobj->f_onEndDragging = [this]
        (uikit::DraggablePanel* dgp)
        {
            onEndDragging();
            if (pcallback->onEndDragging)
            {
                pcallback->onEndDragging(this);
            }
        };
    }

    bool DraggablePanel::draggable() const
    {
        return pimpl->uiobj->isDraggable;
    }

    void DraggablePanel::setDraggable(bool value)
    {
        pimpl->uiobj->isDraggable = value;
    }

    DraggablePanel::Callback& DraggablePanel::callback() const { return *pcallback; }

    void DraggablePanel::onStartDragging() { }

    void DraggablePanel::onEndDragging() { }
}
