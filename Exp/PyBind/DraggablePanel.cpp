#include "Common/Precompile.h"

#include "Callback.h"
#include "DraggablePanel.h"

namespace d14uikit
{
    void Py_InitDraggablePanel(py::module_& m)
    {
        py::class_<DraggablePanel, Panel, PyDraggablePanel<>> i(
            m,
            "DraggablePanel",
            py::multiple_inheritance());

        i.def(py::init<>());

        i.def_property(
            "draggable",
            &DraggablePanel::draggable,
            &DraggablePanel::setDraggable);

        _D14_CALLBACK_PROPERTY(DraggablePanel, onEndDragging);
        _D14_CALLBACK_PROPERTY(DraggablePanel, onStartDragging);

        i.def(
            "onEndDragging",
            &ExDraggablePanel::onEndDragging);

        i.def(
            "onStartDragging",
            &ExDraggablePanel::onStartDragging);
    }
}
