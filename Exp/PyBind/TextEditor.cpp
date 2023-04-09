#include "Common/Precompile.h"

#include "TextEditor.h"

#include "TextInput.h"

namespace d14uikit
{
    void Py_InitTextEditor(py::module_& m)
    {
        py::class_<TextEditor, LabelArea, PyTextEditor<>> i(m, "TextEditor");

        i.def(py::init());

        _D14_UIKIT_PYBIND_TEXT_INPUT(TextEditor)
    }
}
