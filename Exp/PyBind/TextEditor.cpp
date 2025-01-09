#include "Common/Precompile.h"

#include "TextEditor.h"

#include "RawTextInput.h"

#include "Inc/TextEditor.h"

namespace d14uikit
{
    void Py_InitTextEditor(py::module_& m)
    {
        py::class_<TextEditor, TextInput, PyRawTextInput<TextEditor>> i(m, "TextEditor");

        i.def(py::init());
    }
}
