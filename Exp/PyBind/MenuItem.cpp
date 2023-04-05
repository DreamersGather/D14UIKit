#include "Common/Precompile.h"

#include "MenuItem.h"

#include "Panel.h"
#include "TextFormat.h"
#include "ViewItem.h"

#include "Inc/MenuItem.h"
#include "Inc/PopupMenu.h"

namespace d14uikit
{
    void Py_InitMenuItem(py::module_& m)
    {
        py::class_<MenuItem, ViewItem, PyPanel<MenuItem>> i(m, "MenuItem");

        i.def(
            py::init<
            const std::wstring&,
            const std::wstring&>(),
            "labelText"_a = L"ViewItem",
            "hotkeyText"_a = L"");

        i.def_property(
            "associatedMenu",
            &MenuItem::associatedMenu,
            &MenuItem::setAssociatedMenu);

        i.def_property(
            "hotkeyText",
            &MenuItem::hotkeyText,
            &MenuItem::setHotkeyText);

        _D14_UIKIT_PYBIND_TEXT_FORMAT(MenuItem, Hotkey)
    }
}
