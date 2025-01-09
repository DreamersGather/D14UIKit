#include "Common/Precompile.h"

#include "CheckBox.h"
#include "EnumBind.h"

#include "StatefulObject.h"

namespace d14uikit
{
    void Py_InitCheckBox(py::module_& m)
    {
        py::class_<CheckBox, ClickablePanel, PyCheckBox<>> i(m, "CheckBox");

        i.def(py::init<bool>(), "tripleState"_a = false);

        py::enum_<CheckBox::State>(i, "State")
            _D14_UIKIT_PYBIND_ENUM(CheckBox, Unchecked)
            _D14_UIKIT_PYBIND_ENUM(CheckBox, Intermediate)
            _D14_UIKIT_PYBIND_ENUM(CheckBox, Checked)
            .export_values();

        i.def_property("tripleState", &CheckBox::tripleState, &CheckBox::setTripleState);

        _D14_UIKIT_PYBIND_STATEFUL_OBJECT(CheckBox)
    }
}
