#include "Common/Precompile.h"

#include "ToggleButton.h"

#include "EnumBind.h"
#include "StatefulObject.h"

namespace d14uikit
{
    void Py_InitToggleButton(py::module_& m)
    {
        py::class_<ToggleButton, FilledButton, PyToggleButton<>> i(m, "ToggleButton");

        i.def(py::init<const std::wstring&>(), "text"_a = L"ToggleButton");

        py::enum_<ToggleButton::State>(i, "State")
            _D14_UIKIT_PYBIND_ENUM(ToggleButton, Activated)
            _D14_UIKIT_PYBIND_ENUM(ToggleButton, Deactivated)
            .export_values();

        _D14_UIKIT_PYBIND_STATEFUL_OBJECT(ToggleButton)
    }
}
