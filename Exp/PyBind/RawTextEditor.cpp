#include "Common/Precompile.h"

#include "RawTextEditor.h"

#include "RawTextInput.h"

#include "Inc/RawTextEditor.h"

namespace d14uikit
{
    void Py_InitRawTextEditor(py::module_& m)
    {
        py::class_<RawTextEditor, RawTextInput, PyRawTextInput<RawTextEditor>> i(m, "RawTextEditor");

        i.def(py::init());
    }
}
