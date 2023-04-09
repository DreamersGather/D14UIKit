#include "Common/Precompile.h"

#include "RawTextEditor.h"

#include "RawTextInput.h"

namespace d14uikit
{
    void Py_InitRawTextEditor(py::module_& m)
    {
        py::class_<RawTextEditor, LabelArea, PyRawTextEditor<>> i(m, "RawTextEditor");

        i.def(py::init());

        _D14_UIKIT_PYBIND_RAW_TEXT_INPUT(RawTextEditor)
    }
}
