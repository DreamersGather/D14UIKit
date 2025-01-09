#include "Common/Precompile.h"

#include "TextBox.h"

#include "RawTextInput.h"

#include "Inc/TextBox.h"

namespace d14uikit
{
    void Py_InitTextBox(py::module_& m)
    {
        py::class_<TextBox, TextInput, PyRawTextInput<TextBox>> i(m, "TextBox");

        i.def(py::init());
    }
}
