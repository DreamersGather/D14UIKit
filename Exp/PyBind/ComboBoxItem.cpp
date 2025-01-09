#include "Common/Precompile.h"

#include "ComboBoxItem.h"

#include "Panel.h"

#include "Inc/ComboBoxItem.h"

namespace d14uikit
{
    void Py_InitComboBoxItem(py::module_& m)
    {
        py::class_<ComboBoxItem, MenuItem, PyPanel<ComboBoxItem>> i(m, "ComboBoxItem");

        i.def(py::init<const std::wstring&>(), "text"_a = L"ComboBoxItem");
    }
}
