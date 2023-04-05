#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/ClickablePanel.h"

namespace d14uikit
{
    void Py_InitClickablePanel(py::module_& m);

    class ExClickablePanel : public ClickablePanel
    {
    public:
        using ClickablePanel::onMouseButtonPress;
        using ClickablePanel::onMouseButtonRelease;
    };

    template<typename ClickablePanelBase = ClickablePanel>
    class PyClickablePanel : public PyPanel<ClickablePanelBase>
    {
    public:
        using PyPanel::PyPanel;

        void onMouseButtonPress(MouseButtonClickEvent* e) override
        {
            PYBIND11_OVERRIDE(void, ClickablePanelBase, onMouseButtonPress, e);
        }
        void onMouseButtonRelease(MouseButtonClickEvent* e) override
        {
            PYBIND11_OVERRIDE(void, ClickablePanelBase, onMouseButtonRelease, e);
        }
    };
}
