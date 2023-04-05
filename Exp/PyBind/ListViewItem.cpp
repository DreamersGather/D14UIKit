#include "Common/Precompile.h"

#include "ListViewItem.h"

#include "Panel.h"

#include "Inc/ListViewItem.h"

namespace d14uikit
{
    void Py_InitListViewItem(py::module_& m)
    {
        py::class_<ListViewItem, ViewItem, PyPanel<ListViewItem>> i(m, "ListViewItem");

        i.def(py::init<const std::wstring&>(), "text"_a = L"ViewItem");
    }
}
