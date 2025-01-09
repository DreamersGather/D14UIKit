#include "Common/Precompile.h"

#include "TabCaption.h"

#include "Panel.h"

#include "Inc/IconLabel.h"
#include "Inc/TabCaption.h"

namespace d14uikit
{
    void Py_InitTabCaption(py::module_& m)
    {
        py::class_<TabCaption, Panel, PyPanel<TabCaption>> i(m, "TabCaption");

        i.def(py::init<const std::wstring&>(), "title"_a = L"Untitled");

        i.def_property_readonly(
            "title",
            &TabCaption::title);

        i.def_property(
            "text",
            &TabCaption::text,
            &TabCaption::setText);

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
