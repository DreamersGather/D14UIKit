#include "Common/Precompile.h"

#include "ViewItem.h"

#include "Panel.h"
#include "TextFormat.h"

#include "Inc/Image.h"
#include "Inc/ViewItem.h"

namespace d14uikit
{
    void Py_InitViewItem(py::module_& m)
    {
        py::class_<ViewItem, PyPanel<ViewItem>> i(m, "ViewItem");

        i.def(py::init<const std::wstring&>(), "text"_a = L"ViewItem");

        i.def_property(
            "icon",
            &ViewItem::icon,
            &ViewItem::setIcon);

        i.def_property(
            "iconSize",
            &ViewItem::iconSize,
            &ViewItem::setIconSize);

        i.def_property(
            "text",
            &ViewItem::text,
            &ViewItem::setText);

        _D14_UIKIT_PYBIND_TEXT_FORMAT(ViewItem)
    }
}
