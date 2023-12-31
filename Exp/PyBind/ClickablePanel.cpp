﻿#include "Common/Precompile.h"

#include "ClickablePanel.h"

#include "Callback.h"

namespace d14uikit
{
    void Py_InitClickablePanel(py::module_& m)
    {
        py::class_<ClickablePanel, Panel, PyClickablePanel<>> i(
            m,
            "ClickablePanel",
            py::multiple_inheritance());

        i.def(py::init());

        i.def_property(
            "doubleClick",
            &ClickablePanel::doubleClick,
            &ClickablePanel::setDoubleClick);

        _D14_CALLBACK_PROPERTY(ClickablePanel, onMouseButtonPress);
        _D14_CALLBACK_PROPERTY(ClickablePanel, onMouseButtonRelease);

        i.def(
            "onMouseButtonPress",
            &ExClickablePanel::onMouseButtonPress,
            "event"_a);

        i.def(
            "onMouseButtonRelease",
            &ExClickablePanel::onMouseButtonRelease,
            "event"_a);
    }
}
