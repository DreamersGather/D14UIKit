#include "Common/Precompile.h"

#include "RawTextBox.h"

#include "RawTextInput.h"

namespace d14uikit
{
    void Py_InitRawTextBox(py::module_& m)
    {
        py::class_<RawTextBox, LabelArea, PyRawTextBox<>> i(m, "RawTextBox");

        i.def(py::init());

        _D14_UIKIT_PYBIND_RAW_TEXT_INPUT(RawTextBox)
    }
}
