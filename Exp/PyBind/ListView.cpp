#include "Common/Precompile.h"

#include "ListView.h"

#include "ScrollView.h"
#include "WaterfallView.h"

#include "Inc/ListViewItem.h"

namespace d14uikit
{
    void Py_InitListView(py::module_& m)
    {
        py::class_<ListView, ScrollView, PyScrollView<ListView>> i(m, "ListView");

        i.def(py::init());

        _D14_UIKIT_PYBIND_WATERFALL_VIEW(ListView)

        i.def(
            "insertItem",
            &ListView::insertItem,
            "items"_a,
            "index"_a = 0);

        i.def(
            "appendItem",
            &ListView::appendItem,
            "items"_a);

        i.def(
            "removeItem",
            &ListView::removeItem,
            "index"_a,
            "count"_a = 1);

        i.def(
            "clearAllItems",
            &ListView::clearAllItems);
    }
}
