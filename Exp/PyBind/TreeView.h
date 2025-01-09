#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/TreeView.h"

namespace d14uikit
{
    void Py_InitTreeView(py::module_& m);

    class ExTreeView : public TreeView
    {
    public:
        using TreeView::onSelectChange;
    };

    template<typename TreeViewBase = TreeView>
    class PyTreeView : public PyPanel<TreeViewBase>
    {
    public:
        using PyPanel<TreeViewBase>::PyPanel;

        void onSelectChange(const std::set<int>& selected) override
        {
            PYBIND11_OVERRIDE(void, TreeViewBase, onSelectChange, selected);
        }
    };
}
