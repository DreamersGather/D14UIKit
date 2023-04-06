#include "Common/Precompile.h"

#include "ScrollView.h"

#include "Callback.h"

namespace d14uikit
{
    void Py_InitScrollView(py::module_& m)
    {
        py::class_<ScrollView, ResizablePanel, PyScrollView<>> i(m, "ScrollView");

        i.def(py::init());

        i.def_property(
            "horzDeltaPixel",
            &ScrollView::horzDeltaPixel,
            &ScrollView::setHorzDeltaPixel);

        i.def_property(
            "vertDeltaPixel",
            &ScrollView::vertDeltaPixel,
            &ScrollView::setVertDeltaPixel);

        i.def_property(
            "content",
            &ScrollView::content,
            &ScrollView::setContent);

        i.def_property(
            "viewportOffset",
            &ScrollView::viewportOffset,
            &ScrollView::setViewportOffset);

#define DEF_PROPERTY(Name) \
_D14_CALLBACK_PROPERTY(ScrollView, Name)

        DEF_PROPERTY(onEndThumbScrolling);
        DEF_PROPERTY(onStartThumbScrolling);
        DEF_PROPERTY(onViewportOffsetChange);

#undef DEF_PROPERTY

        i.def(
            "onEndThumbScrolling",
            &ExScrollView::onEndThumbScrolling,
            "offset"_a);

        i.def(
            "onStartThumbScrolling",
            &ExScrollView::onStartThumbScrolling,
            "offset"_a);

        i.def(
            "onViewportOffsetChange",
            &ExScrollView::onViewportOffsetChange,
            "offset"_a);
    }
}
