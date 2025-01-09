#include "Common/Precompile.h"

#include "ElevatedButton.h"

#include "ClickablePanel.h"

#include "Inc/ElevatedButton.h"

namespace d14uikit
{
    void Py_InitElevatedButton(py::module_& m)
    {
        py::class_<ElevatedButton, FilledButton, PyClickablePanel<ElevatedButton>> i(m, "ElevatedButton");

        i.def(py::init<const std::wstring&>(), "text"_a = L"ElevatedButton");
    }
}
