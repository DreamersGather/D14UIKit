﻿#include "Common/Precompile.h"

#include "ResizablePanel.h"

#include "Common.h"

#include "UIKit/ResizablePanel.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    ResizablePanel::ResizablePanel()
        :
        ResizablePanel(uikit::makeUIObject<uikit::ResizablePanel>(
            D2D1_RECT_F{}, uikit::resource_utils::g_solidColorBrush)) { }

    _D14_UIKIT_CTOR(ResizablePanel)
        :
        Panel(uiobj),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>())
    {
        pimpl->uiobj = uiobj;

        pimpl->uiobj->f_onEndResizing = [this]
        (uikit::ResizablePanel* rsp)
        {
            onEndResizing();
            if (pcallback->onEndResizing)
            {
                pcallback->onEndResizing(this);
            }
        };
        pimpl->uiobj->f_onStartResizing = [this]
        (uikit::ResizablePanel* rsp)
        {
            onStartResizing();
            if (pcallback->onStartResizing)
            {
                pcallback->onStartResizing(this);
            }
        };
    }

    bool ResizablePanel::resizable(Border border) const
    {
        switch (border)
        {
        case All:
        {
            return
            pimpl->uiobj->isLeftResizable &&
            pimpl->uiobj->isRightResizable &&
            pimpl->uiobj->isTopResizable &&
            pimpl->uiobj->isBottomResizable;
        }
        case Left: return pimpl->uiobj->isLeftResizable;
        case Right: return pimpl->uiobj->isRightResizable;
        case Top: return pimpl->uiobj->isTopResizable;
        case Bottom: return pimpl->uiobj->isBottomResizable;
        default: return false;
        }
    }

    void ResizablePanel::setResizable(bool value, Border border)
    {
        switch (border)
        {
        case All:
        {
            pimpl->uiobj->isLeftResizable =
            pimpl->uiobj->isRightResizable =
            pimpl->uiobj->isTopResizable =
            pimpl->uiobj->isBottomResizable = value;
            break;
        }
        case Left: pimpl->uiobj->isLeftResizable = value; break;
        case Right: pimpl->uiobj->isRightResizable = value; break;
        case Top: pimpl->uiobj->isTopResizable = value; break;
        case Bottom: pimpl->uiobj->isBottomResizable = value; break;
        default: break;
        }
    }

    bool ResizablePanel::dynamicSizing() const
    {
        return pimpl->uiobj->enableDynamicSizing;
    }

    void ResizablePanel::setDynamicSizing(bool value)
    {
        pimpl->uiobj->enableDynamicSizing = value;
    }

    ResizablePanel::Callback& ResizablePanel::callback() const { return *pcallback; }

    void ResizablePanel::onEndResizing() {}

    void ResizablePanel::onStartResizing() { }
}
