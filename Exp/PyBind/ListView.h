#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

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
    class PyListView : public PyPanel<ListViewBase>
    {
    public:
        using PyPanel::PyPanel;

        void onSelectChange(const std::set<int> selected) override
        {
            PYBIND11_OVERRIDE(void, ListViewBase, onSelectChange, selected);
        }
    };
}
