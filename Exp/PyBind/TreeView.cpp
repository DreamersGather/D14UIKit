#include "Common/Precompile.h"

#include "TreeView.h"

#include "ScrollView.h"

#include "Inc/TreeView.h"
#include "Inc/TreeViewItem.h"

namespace d14uikit
{
    void Py_InitTreeView(py::module_& m)
    {
        py::class_<TreeView, ScrollView, PyScrollView<TreeView>> i(m, "TreeView");

        i.def(py::init());

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
    }
}
