#pragma once

#include "ResizablePanel.h"

_D14_UIKIT_FWDEF(ScrollView)

namespace d14uikit
{
    class DllExport ScrollView : public ResizablePanel
    {
        _D14_UIKIT_PIMPL(ScrollView)

        ScrollView();

        int horzDeltaPixel() const;
        void setHorzDeltaPixel(int value);

        int vertDeltaPixel() const;
        void setVertDeltaPixel(int value);

        // The return value may be null
        Panel* content() const;
        void setContent(Panel* uiobj);

        Point viewportOffset() const;
        void setViewportOffset(const Point& offset);

        struct Callback
        {
            std::function<void(ScrollView*, const Point&)> onEndThumbScrolling = {};

            std::function<void(ScrollView*, const Point&)> onStartThumbScrolling = {};

            std::function<void(ScrollView*, const Point&)> onViewportOffsetChange = {};
        };
        Callback& callback() const;

    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void onEndThumbScrolling(const Point& offset);

        virtual void onStartThumbScrolling(const Point& offset);

        virtual void onViewportOffsetChange(const Point& offset);
    };
}
