#include "Common/Precompile.h"

#include "DraggablePanel.h"

#include "Common.h"

#include "UIKit/DraggablePanel.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    DraggablePanel::DraggablePanel()
        :
        DraggablePanel(uikit::makeUIObject<uikit::DraggablePanel>(
            D2D1_RECT_F{}, uikit::resource_utils::g_solidColorBrush)) { }

    _D14_UIKIT_CTOR(DraggablePanel)
        :
        Panel(uiobj),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>())
    {
        pimpl->uiobj = uiobj;

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

    void DraggablePanel::onEndDragging() { }

    void DraggablePanel::onStartDragging() { }
}
