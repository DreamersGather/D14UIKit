#include "Common/Precompile.h"

#include "ClickablePanel.h"
#include "OutlinedButton.h"

#include "Inc/OutlinedButton.h"

namespace d14uikit
{
    void Py_InitOutlinedButton(py::module_& m)
    {
        py::class_<OutlinedButton, FlatButton, PyClickablePanel<OutlinedButton>> i(m, "OutlinedButton");

        i.def(py::init<const std::wstring&>(), "text"_a = L"Button");
    }
}
