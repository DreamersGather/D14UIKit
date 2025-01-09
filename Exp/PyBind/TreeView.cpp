#include "Common/Precompile.h"

#include "TreeView.h"

#include "ScrollView.h"
#include "WaterfallView.h"

#include "Inc/TreeViewItem.h"

namespace d14uikit
{
    void Py_InitTreeView(py::module_& m)
    {
        py::class_<TreeView, ScrollView, PyScrollView<TreeView>> i(m, "TreeView");

        _D14_UIKIT_PYBIND_WATERFALL_VIEW(TreeView)

        i.def_property_readonly(
            "rootItemCount",
            &TreeView::rootItemCount);

        i.def(
            "getRootItem",
            &TreeView::getRootItem,
            "rootIndex"_a);

        i.def(
            "insertRootItem",
            &TreeView::insertRootItem,
            "rootItems"_a,
            "rootIndex"_a = 0);

        i.def(
            "appendRootItem",
            &TreeView::appendRootItem,
            "rootItems"_a);

        i.def(
            "removeRootItem",
            &TreeView::removeRootItem,
            "rootIndex"_a,
            "count"_a = 1);

        i.def(
            "clearAllItems",
            &TreeView::clearAllItems);
    }
}
