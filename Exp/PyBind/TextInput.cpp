#include "Common/Precompile.h"

#include "TextInput.h"

#include "RawTextInput.h"

#include "Inc/TextInput.h"

namespace d14uikit
{
    void Py_InitTextInput(py::module_& m)
    {
        py::class_<TextInput, RawTextInput, PyRawTextInput<TextInput>> i(m, "TextInput");

        i.def(py::init<bool>(), "multiline"_a);
    }
}
