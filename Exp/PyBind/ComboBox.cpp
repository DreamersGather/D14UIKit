#include "Common/Precompile.h"

#include "ComboBox.h"

#include "Callback.h"

#include "Inc/MenuItem.h"
#include "Inc/PopupMenu.h"

namespace d14uikit
{
    void Py_InitComboBox(py::module_& m)
    {
        py::class_<ComboBox, FlatButton, PyComboBox<>> i(m, "ComboBox");

        i.def(py::init());

        i.def_property_readonly("currSelected", &ComboBox::currSelected);
        i.def("setCurrSelected", &ComboBox::setCurrSelected, "index"_a);

        i.def_property_readonly("currSelectedText", &ComboBox::currSelectedText);

        i.def_property_readonly("dropDownMenu", &ComboBox::dropDownMenu);

        _D14_CALLBACK_PROPERTY(ComboBox, onSelectedChange);

        i.def("onSelectedChange", &ExComboBox::onSelectedChange, "text"_a);
    }
}
