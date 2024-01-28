#pragma once

#include "Common/Precompile.h"

#include "ScrollView.h"

#include "Inc/ListView.h"

namespace d14uikit
{
    void Py_InitListView(py::module_& m);

    class ExListView : public ListView
    {
    public:
        using ListView::onSelectChange;
    };

    template<typename ListViewBase = ListView>
    class PyListView : public PyScrollView<ListViewBase>
    {
    public:
        using PyScrollView::PyScrollView;

        void onSelectChange(const std::set<int> selected) override
        {
            PYBIND11_OVERRIDE(void, ListViewBase, onSelectChange, selected);
        }
    };
}
