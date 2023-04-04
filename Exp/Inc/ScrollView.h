#pragma once

#include "ResizablePanel.h"

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

        Panel* content() const;
        void setContent(Panel* uiobj);

        Point viewportOffset() const;
        void setViewportOffset(const Point& offset);

        struct Callback
        {
            std::function<void(ScrollView*)> onEndThumbScrolling = {};

            std::function<void(ScrollView*)> onStartThumbScrolling = {};

            std::function<void(ScrollView*, const Point&)> onViewportOffsetChange = {};
        };
        Callback& callback() const;

    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void onEndThumbScrolling();

        virtual void onStartThumbScrolling();

        virtual void onViewportOffsetChange(const Point& offset);
    };
}
