#include "Common/Precompile.h"

#include "ScrollView.h"

#include "Common.h"
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
        (uikit::ScrollView* sv, const D2D1_POINT_2F& offset)
        {
            auto offset2 = convert(offset);
            onStartThumbScrolling(offset2);
            if (pcallback->onStartThumbScrolling)
            {
                pcallback->onStartThumbScrolling(this, offset2);
            }
        };
        pimpl->uiobj->f_onEndThumbScrolling = [this]
        (uikit::ScrollView* sv, const D2D1_POINT_2F& offset)
        {
            auto offset2 = convert(offset);
            onEndThumbScrolling(offset2);
            if (pcallback->onEndThumbScrolling)
            {
                pcallback->onEndThumbScrolling(this, offset2);
            }
        };
        pimpl->uiobj->f_onViewportOffsetChange = [this]
        (uikit::ScrollView* sv, const D2D1_POINT_2F& offset)
        {
            auto offset2 = convert(offset);
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

    Panel* ScrollView::content() const { return pimpl->content; }

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
        return convert(pimpl->uiobj->viewportOffset());
    }

    void ScrollView::setViewportOffset(const Point& offset)
    {
        pimpl->uiobj->setViewportOffset(convert(offset));
    }

    ScrollView::Callback& ScrollView::callback() const { return *pcallback; }

    void ScrollView::onEndThumbScrolling(const Point& offset) { }

    void ScrollView::onStartThumbScrolling(const Point& offset) { }

    void ScrollView::onViewportOffsetChange(const Point& offset) { }
}
