#include "Common/Precompile.h"

#include "ResizablePanel.h"

#include "Callback.h"

namespace d14uikit
{
    void Py_InitResizablePanel(py::module_& m)
    {
        py::class_<ResizablePanel, Panel, PyResizablePanel<>> i(
            m,
            "ResizablePanel",
            py::multiple_inheritance());

        i.def(py::init());

        py::enum_<ResizablePanel::Border>(i, "Border")
            .value("All", ResizablePanel::All)
            .value("Left", ResizablePanel::Left)
            .value("Right", ResizablePanel::Right)
            .value("Top", ResizablePanel::Top)
            .value("Bottom", ResizablePanel::Bottom)
            .export_values();

        i.def(
            "resizable",
            &ResizablePanel::resizable,
            "border"_a = ResizablePanel::All);

        i.def(
            "setResizable",
            &ResizablePanel::setResizable,
            "value"_a,
            "border"_a = ResizablePanel::All);

        i.def_property(
            "dynamicSizing",
            &ResizablePanel::dynamicSizing,
            &ResizablePanel::setDynamicSizing);

        _D14_CALLBACK_PROPERTY(ResizablePanel, onEndResizing);
        _D14_CALLBACK_PROPERTY(ResizablePanel, onStartResizing);

        i.def(
            "onEndResizing",
            &ExResizablePanel::onEndResizing);

        i.def(
            "onStartResizing",
            &ExResizablePanel::onStartResizing);
    }
}
