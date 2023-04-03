#include "Common/Precompile.h"

#include "FlatButton.h"

#include "ClickablePanel.h"

#include "Inc/FlatButton.h"

namespace d14uikit
{
    void Py_InitFlatButton(py::module_& m)
    {
        py::class_<FlatButton, Button, PyClickablePanel<FlatButton>> i(m, "FlatButton");

        i.def(py::init<const std::wstring&>(), "text"_a = L"Button");
    }
}
