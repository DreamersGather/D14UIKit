#include "Common/Precompile.h"

#include "Button.h"
#include "ClickablePanel.h"

#include "Inc/Button.h"
#include "Inc/Image.h"

namespace d14uikit
{
    void Py_InitButton(py::module_& m)
    {
        py::class_<Button, ClickablePanel, PyClickablePanel<Button>> i(m, "Button");

        i.def(py::init<const std::wstring&>(), "text"_a = L"Button");

        i.def_property(
            "icon",
            &Button::icon,
            &Button::setIcon);

        i.def_property(
            "iconSize",
            &Button::iconSize,
            &Button::setIconSize);

        i.def_property(
            "customIconSize",
            &Button::customIconSize,
            &Button::setCustomIconSize);

        i.def_property(
            "text",
            &Button::text,
            &Button::setText);
    }
}
