#include "Common/Precompile.h"

#include "Button.h"

#include "ClickablePanel.h"

#include "Inc/Button.h"
#include "Inc/IconLabel.h"

namespace d14uikit
{
    void Py_InitButton(py::module_& m)
    {
        py::class_<Button, ClickablePanel, PyClickablePanel<Button>> i(m, "Button");

        i.def(py::init<const std::wstring&>(), "text"_a = L"Button");

        i.def_property_readonly("content", &Button::content);

        i.def_property("text", &Button::text, &Button::setText);
    }
}
