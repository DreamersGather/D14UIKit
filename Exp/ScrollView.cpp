#include "Common/Precompile.h"

#include "ScrollView.h"

#include "Panel.h"
#include "ResizablePanel.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/ScrollView.h"

using namespace d14engine;

namespace d14uikit
{
    ScrollView::ScrollView()
        :
        ScrollView(Passkey{})
    {
        Panel::pimpl->uiobj =
        ResizablePanel::pimpl->uiobj =
        ScrollView::pimpl->uiobj =
        uikit::makeUIObject<uikit::ScrollView>();

        Panel::initialize();
        ResizablePanel::initialize();
        ScrollView::initialize();
    }

    ScrollView::ScrollView(Passkey)
        :
        Panel(Panel::Passkey{}),
        ResizablePanel(ResizablePanel::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void ScrollView::initialize()
    {
        pimpl->uiobj->f_onStartThumbScrolling = [this]
        (uikit::ScrollView* sv)
        {
            onStartResizing();
            if (pcallback->onStartThumbScrolling)
            {
                pcallback->onStartThumbScrolling(this);
            }
        };
        pimpl->uiobj->f_onEndThumbScrolling = [this]
        (uikit::ScrollView* sv)
        {
            onEndResizing();
            if (pcallback->onEndThumbScrolling)
            {
                pcallback->onEndThumbScrolling(this);
            }
        };
        pimpl->uiobj->f_onViewportOffsetChange = [this]
        (uikit::ScrollView* sv, const D2D1_POINT_2F& offset)
        {
            Point offset2 =
            {
                math_utils::round(offset.x),
                math_utils::round(offset.y)
            };
            onViewportOffsetChange(offset2);
            if (pcallback->onViewportOffsetChange)
            {
                pcallback->onViewportOffsetChange(this, offset2);
            }
        };
    }

    int ScrollView::horzDeltaPixel() const
    {
        return math_utils::round(pimpl->uiobj->deltaPixelsPerScroll.horz);
    }

    void ScrollView::setHorzDeltaPixel(int value)
    {
        pimpl->uiobj->deltaPixelsPerScroll.horz = (float)value;
    }

    int ScrollView::vertDeltaPixel() const
    {
        return math_utils::round(pimpl->uiobj->deltaPixelsPerScroll.vert);
    }

    void ScrollView::setVertDeltaPixel(int value)
    {
        pimpl->uiobj->deltaPixelsPerScroll.vert = (float)value;
    }

    Panel* ScrollView::content() const
    {
        return pimpl->content;
    }

    void ScrollView::setContent(Panel* uiobj)
    {
        pimpl->content = uiobj;
        if (uiobj != nullptr)
        {
            pimpl->uiobj->setContent(uiobj->getImpl()->uiobj);
        }
        else pimpl->uiobj->setContent(nullptr);
    }

    Point ScrollView::viewportOffset() const
    {
        auto& offset = pimpl->uiobj->viewportOffset();
        return
        {
            math_utils::round(offset.x),
            math_utils::round(offset.y)
        };
    }

    void ScrollView::setViewportOffset(const Point& offset)
    {
        pimpl->uiobj->setViewportOffset({ (float)offset.x, (float)offset.y });
    }

    ScrollView::Callback& ScrollView::callback() const { return *pcallback; }

    void ScrollView::onEndThumbScrolling() { }

    void ScrollView::onStartThumbScrolling() { }

    void ScrollView::onViewportOffsetChange(const Point& offset) { }
}
