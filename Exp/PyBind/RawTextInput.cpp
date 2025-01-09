#include "Common/Precompile.h"

#include "RawTextInput.h"

#include "Callback.h"

#include "Inc/Label.h"

namespace d14uikit
{
    void Py_InitRawTextInput(py::module_& m)
    {
        py::class_<RawTextInput, LabelArea, PyRawTextInput<>> i(m, "RawTextInput");

        i.def(py::init<bool>(), "multiline"_a);

        i.def_property("editable", &RawTextInput::editable, &RawTextInput::setEditable);

        i.def_property("textRect", &RawTextInput::textRect, &RawTextInput::setTextRect);

        i.def_property_readonly("placeholder", &RawTextInput::placeholder);

        i.def("performCommandCtrlX", &RawTextInput::performCommandCtrlX);

        i.def("performCommandCtrlV", &RawTextInput::performCommandCtrlV);

        _D14_CALLBACK_PROPERTY(RawTextInput, onTextChange);

        i.def("onTextChange", &ExRawTextInput::onTextChange, "text"_a);

    }
}
