#include "Common/Precompile.h"

#include "ViewItem.h"

#include "Panel.h"

#include "Inc/IconLabel.h"
#include "Inc/ViewItem.h"

namespace d14uikit
{
    void Py_InitViewItem(py::module_& m)
    {
        py::class_<ViewItem, Panel, PyPanel<ViewItem>> i(m, "ViewItem");

        i.def(py::init<const std::wstring&>(), "text"_a = L"ViewItem");

        i.def_property_readonly(
            "content",
            &ViewItem::content);

        i.def_property(
            "text",
            &ViewItem::text,
            &ViewItem::setText);
    }
}
