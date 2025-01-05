#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/DraggablePanel.h"

namespace d14uikit
{
    void Py_InitDraggablePanel(py::module_& m);

    class ExDraggablePanel : public DraggablePanel
    {
    public:
        using DraggablePanel::onEndDragging;
        using DraggablePanel::onStartDragging;
    };

    template<typename DraggablePanelBase = DraggablePanel>
    class PyDraggablePanel : public PyPanel<DraggablePanelBase>
    {
    public:
        using PyPanel<DraggablePanelBase>::PyPanel;

        void onEndDragging() override
        {
            PYBIND11_OVERRIDE(void, DraggablePanelBase, onEndDragging, );
        }
        void onStartDragging() override
        {
            PYBIND11_OVERRIDE(void, DraggablePanelBase, onStartDragging, );
        }
    };
}
