#include "Common/Precompile.h"

#include "TextBox.h"

#include "TextInput.h"

namespace d14uikit
{
    void Py_InitTextBox(py::module_& m)
    {
        py::class_<TextBox, LabelArea, PyTextBox<>> i(m, "TextBox");

        i.def(py::init());

        _D14_UIKIT_PYBIND_TEXT_INPUT(TextBox)
    }
}
