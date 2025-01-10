#include "Common/Precompile.h"

#include "IconLabel.h"

#include "Panel.h"

#include "Inc/IconLabel.h"
#include "Inc/Image.h"
#include "Inc/Label.h"

namespace d14uikit
{
    void Py_InitIconLabel(py::module_& m)
    {
        py::class_<IconLabel, Panel, PyPanel<IconLabel>> i(m, "IconLabel");

        i.def(py::init<const std::wstring&>(), "text"_a = L"");

        i.def_property(
            "icon",
            &IconLabel::icon,
            &IconLabel::setIcon);

        i.def_property(
            "iconSize",
            &IconLabel::iconSize,
            &IconLabel::setIconSize);

        i.def_property_readonly(
            "label",
            &IconLabel::label);

        i.def(
            "updateLayout",
            &IconLabel::updateLayout);
    }
}
