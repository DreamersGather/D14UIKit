#include "Common/Precompile.h"

#include "CheckBox.h"

#include "Callback.h"

namespace d14uikit
{
    void Py_InitCheckBox(py::module_& m)
    {
        py::class_<CheckBox, ClickablePanel, PyCheckBox<>> i(m, "CheckBox");

        i.def(py::init<bool>(), "tripleState"_a = false);

        py::enum_<CheckBox::State>(i, "State")
            .value("Unchecked", CheckBox::Unchecked)
            .value("Intermediate", CheckBox::Intermediate)
            .value("Checked", CheckBox::Checked)
            .export_values();

        i.def_property("state", &CheckBox::state, &CheckBox::setState);

        i.def_property("tripleState", &CheckBox::tripleState, &CheckBox::setTripleState);

        _D14_CALLBACK_PROPERTY(CheckBox, onStateChange);

        i.def("onStateChange", &ExCheckBox::onStateChange, "state"_a);
    }
}
