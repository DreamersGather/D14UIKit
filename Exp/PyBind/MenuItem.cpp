#include "Common/Precompile.h"

#include "MenuItem.h"

#include "Panel.h"
#include "ViewItem.h"

#include "Inc/Label.h"
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
            "labelText"_a = L"MenuItem",
            "hotkeyText"_a = L"");

        i.def_property_readonly(
            "hotkey",
            &MenuItem::hotkey);

        i.def_property(
            "associatedMenu",
            &MenuItem::associatedMenu,
            &MenuItem::setAssociatedMenu);

        i.def_property(
            "instant",
            &MenuItem::instant,
            &MenuItem::setInstant);

        i.def_property(
            "trigger",
            &MenuItem::trigger,
            &MenuItem::setTrigger);
    }
}
