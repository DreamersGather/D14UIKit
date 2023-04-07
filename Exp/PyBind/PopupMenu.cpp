#include "Common/Precompile.h"

#include "PopupMenu.h"

#include "Callback.h"

#include "Inc/MenuItem.h"

namespace d14uikit
{
    void Py_InitPopupMenu(py::module_& m)
    {
        py::class_<PopupMenu, ScrollView, PyPopupMenu<>> i(m, "PopupMenu");

        i.def(py::init());

        i.def_property(
            "roundExtension",
            &PopupMenu::roundExtension,
            &PopupMenu::setRoundExtension);

        i.def(
            "insertItem",
            &PopupMenu::insertItem,
            "items"_a,
            "index"_a = 0);

        i.def(
            "appendItem",
            &PopupMenu::appendItem,
            "items"_a);

        i.def(
            "removeItem",
            &PopupMenu::removeItem,
            "index"_a,
            "count"_a = 1);

        i.def(
            "clearAllItems",
            &PopupMenu::clearAllItems);

        i.def(
            "setActivated",
            &PopupMenu::setActivated);

        i.def(
            "setBkgnTriggerPanel",
            &PopupMenu::setBkgnTriggerPanel);

        _D14_CALLBACK_PROPERTY(PopupMenu, onTriggerMenuItem);

        i.def("onTriggerMenuItem", &ExPopupMenu::onTriggerMenuItem);
    }
}
