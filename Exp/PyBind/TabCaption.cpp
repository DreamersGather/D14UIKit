#include "Common/Precompile.h"

#include "TabCaption.h"

#include "Panel.h"
#include "TextFormat.h"

#include "Inc/Image.h"
#include "Inc/TabCaption.h"

namespace d14uikit
{
    void Py_InitTabCaption(py::module_& m)
    {
        py::class_<TabCaption, Panel, PyPanel<TabCaption>> i(m, "TabCaption");

        i.def(py::init<const std::wstring&>(), "title"_a = L"Untitled");

        i.def_property(
            "icon",
            &TabCaption::icon,
            &TabCaption::setIcon);

        i.def_property(
            "iconSize",
            &TabCaption::iconSize,
            &TabCaption::setIconSize);

        i.def_property(
            "title",
            &TabCaption::title,
            &TabCaption::setTitle);

        _D14_UIKIT_PYBIND_TEXT_FORMAT(TabCaption)

        i.def_property(
            "closable",
            &TabCaption::closable,
            &TabCaption::setClosable);

        i.def_property(
            "draggable",
            &TabCaption::draggable,
            &TabCaption::setDraggable);

        i.def_property(
            "promotable",
            &TabCaption::promotable,
            &TabCaption::setPromotable);
    }
}
