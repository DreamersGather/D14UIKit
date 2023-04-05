#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/TreeViewItem.h"

namespace d14uikit
{
    void Py_InitTreeViewItem(py::module_& m);

    class ExTreeViewItem : public TreeViewItem
    {
    public:
        using TreeViewItem::onStateChange;
    };

    template<typename TreeViewItemBase = TreeViewItem>
    class PyTreeViewItem : public PyPanel<TreeViewItemBase>
    {
    public:
        using PyPanel::PyPanel;

        void onStateChange(TreeViewItem::State state) override
        {
            PYBIND11_OVERRIDE(void, TreeViewItemBase, onStateChange, state);
        }
    };
}
