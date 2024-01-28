#pragma once

#include "Common/Precompile.h"

#include "ResizablePanel.h"

#include "Inc/TabGroup.h"

namespace d14uikit
{
    void Py_InitTabGroup(py::module_& m);

    class ExTabGroup : public TabGroup
    {
    public:
        using TabGroup::onSelectedChange;
    };

    template<typename TabGroupBase = TabGroup>
    class PyTabGroup : public PyResizablePanel<TabGroupBase>
    {
    public:
        using PyResizablePanel::PyResizablePanel;

        void onSelectedChange(const std::wstring& title) override
        {
            PYBIND11_OVERRIDE(void, TabGroupBase, onSelectedChange, title);
        }
    };
}
