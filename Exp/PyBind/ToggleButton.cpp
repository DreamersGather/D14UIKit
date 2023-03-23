#include "Common/Precompile.h"

#include "Callback.h"
#include "ToggleButton.h"

namespace d14uikit
{
    void Py_InitToggleButton(py::module_& m)
    {
        py::class_<ToggleButton, FilledButton, PyToggleButton<>> i(m, "ToggleButton");

        i.def(py::init<const std::wstring&>(), "text"_a = L"Button");

        py::enum_<ToggleButton::State>(i, "State")
            .value("Activated", ToggleButton::Activated)
            .value("Deactivated", ToggleButton::Deactivated)
            .export_values();

        i.def_property("state", &ToggleButton::state, &ToggleButton::setState);

        _D14_CALLBACK_PROPERTY(ToggleButton, onStateChange);

        i.def("onStateChange", &ExToggleButton::onStateChange);
    }
}
