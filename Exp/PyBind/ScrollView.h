#pragma once

#include "Common/Precompile.h"

#include "ResizablePanel.h"

#include "Inc/ScrollView.h"

namespace d14uikit
{
    void Py_InitScrollView(py::module_& m);

    class ExScrollView : public d14uikit::ScrollView
    {
    public:
        using ScrollView::onEndThumbScrolling;
        using ScrollView::onStartThumbScrolling;
        using ScrollView::onViewportOffsetChange;
    };

    template<typename ScrollViewBase = ScrollView>
    class PyScrollView : public PyResizablePanel<ScrollViewBase>
    {
    public:
        using PyResizablePanel::PyResizablePanel;

        void onEndThumbScrolling(const Point& offset) override
        {
            PYBIND11_OVERRIDE(void, ScrollViewBase, onEndThumbScrolling, offset);
        }
        void onStartThumbScrolling(const Point& offset) override
        {
            PYBIND11_OVERRIDE(void, ScrollViewBase, onStartThumbScrolling, offset);
        }
        void onViewportOffsetChange(const Point& offset) override
        {
            PYBIND11_OVERRIDE(void, ScrollViewBase, onViewportOffsetChange, offset);
        }
    };
}
