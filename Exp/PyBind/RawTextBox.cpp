#include "Common/Precompile.h"

#include "RawTextBox.h"

#include "RawTextInput.h"

#include "Inc/RawTextBox.h"

namespace d14uikit
{
    void Py_InitRawTextBox(py::module_& m)
    {
        py::class_<RawTextBox, RawTextInput, PyRawTextInput<RawTextBox>> i(m, "RawTextBox");

        i.def(py::init());
    }
}
