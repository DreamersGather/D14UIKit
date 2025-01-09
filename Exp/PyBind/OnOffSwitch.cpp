#include "Common/Precompile.h"

#include "OnOffSwitch.h"

#include "EnumBind.h"
#include "StatefulObject.h"

namespace d14uikit
{
    void Py_InitOnOffSwitch(py::module_& m)
    {
        py::class_<OnOffSwitch, ClickablePanel, PyOnOffSwitch<>> i(m, "OnOffSwitch");

        i.def(py::init());

        py::enum_<OnOffSwitch::State>(i, "State")
            _D14_UIKIT_PYBIND_ENUM(OnOffSwitch, On)
            _D14_UIKIT_PYBIND_ENUM(OnOffSwitch, Off)
            .export_values();

        _D14_UIKIT_PYBIND_STATEFUL_OBJECT(OnOffSwitch)
    }
}
