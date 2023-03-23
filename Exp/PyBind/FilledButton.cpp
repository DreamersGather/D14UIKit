#include "Common/Precompile.h"

#include "ClickablePanel.h"
#include "FilledButton.h"

#include "Inc/FilledButton.h"

namespace d14uikit
{
    void Py_InitFilledButton(py::module_& m)
    {
        py::class_<FilledButton, FlatButton, PyClickablePanel<FilledButton>> i(m, "FilledButton");

        i.def(py::init<const std::wstring&>(), "text"_a = L"Button");
    }
}
