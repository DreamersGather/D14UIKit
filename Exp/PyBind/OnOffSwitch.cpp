#include "Common/Precompile.h"

#include "OnOffSwitch.h"

#include "Callback.h"

namespace d14uikit
{
    void Py_InitOnOffSwitch(py::module_& m)
    {
        py::class_<OnOffSwitch, ClickablePanel, PyOnOffSwitch<>> i(m, "OnOffSwitch");

        i.def(py::init());

        py::enum_<OnOffSwitch::State>(i, "State")
            .value("On", OnOffSwitch::On)
            .value("Off", OnOffSwitch::Off)
            .export_values();

        i.def_property("state", &OnOffSwitch::state, &OnOffSwitch::setState);

        _D14_CALLBACK_PROPERTY(OnOffSwitch, onStateChange);

        i.def("onStateChange", &ExOnOffSwitch::onStateChange, "state"_a);
    }
}
