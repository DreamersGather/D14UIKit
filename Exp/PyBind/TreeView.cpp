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

        i.def(py::init());

        _D14_UIKIT_PYBIND_WATERFALL_VIEW(TreeView)

        i.def(
            "insertRootItem",
            &TreeView::insertRootItem);

        i.def(
            "appendRootItem",
            &TreeView::appendRootItem);

        i.def(
            "removeRootItem",
            &TreeView::removeRootItem);

        i.def(
            "clearAllItems",
            &TreeView::clearAllItems);

        i.def_property_readonly(
            "rootItemCount",
            &TreeView::rootItemCount);
    }
}
