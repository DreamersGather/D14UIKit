﻿#include "Common/Precompile.h"

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

        i.def_property(
            "resizable",
            &ResizablePanel::resizable,
            &ResizablePanel::setResizable);

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
