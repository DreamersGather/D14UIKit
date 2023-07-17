#include "Common/Precompile.h"

#include "TreeViewItem.h"

#include "Callback.h"

namespace d14uikit
{
    void Py_InitTreeViewItem(py::module_& m)
    {
        py::class_<TreeViewItem, ViewItem, PyTreeViewItem<>> i(m, "TreeViewItem");

        i.def(py::init<const std::wstring&>(), "text"_a = L"ViewItem");

        i.def_property_readonly(
            "nodeLevel",
            &TreeViewItem::nodeLevel);

        i.def(
            "insertItem",
            &TreeViewItem::insertItem,
            "items"_a,
            "index"_a = 0);

        i.def(
            "appendItem",
            &TreeViewItem::appendItem,
            "items"_a);

        i.def(
            "removeItem",
            &TreeViewItem::removeItem,
            "index"_a,
            "count"_a = 1);

        i.def(
            "clearAllItems",
            &TreeViewItem::clearAllItems);

        i.def_property_readonly(
            "itemCount",
            &TreeViewItem::itemCount);

        i.def_property(
            "state",
            &TreeViewItem::state,
            &TreeViewItem::setState);

        _D14_CALLBACK_PROPERTY(TreeViewItem, onStateChange);

        i.def("onStateChange", &ExTreeViewItem::onStateChange, "state"_a);
    }
}
