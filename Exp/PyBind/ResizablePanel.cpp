#include "Common/Precompile.h"

#include "ResizablePanel.h"

#include "Callback.h"
#include "EnumBind.h"

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
            _D14_UIKIT_PYBIND_ENUM(ResizablePanel, All)
            _D14_UIKIT_PYBIND_ENUM(ResizablePanel, Left)
            _D14_UIKIT_PYBIND_ENUM(ResizablePanel, Right)
            _D14_UIKIT_PYBIND_ENUM(ResizablePanel, Top)
            _D14_UIKIT_PYBIND_ENUM(ResizablePanel, Bottom)
            .export_values();

        i.def_property(
            "resizable",
            [](ResizablePanel& self)
            {
                return self.resizable();
            },
            [](ResizablePanel& self, bool value)
            {
                self.setResizable(value);
            });
        i.def(
            "getResizable",
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
