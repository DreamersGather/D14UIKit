﻿#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/ResizablePanel.h"

namespace d14uikit
{
    void Py_InitResizablePanel(py::module_& m);

    class ExResizablePanel : public ResizablePanel
    {
    public:
        using ResizablePanel::onEndResizing;
        using ResizablePanel::onStartResizing;
    };

    template<typename ResizablePanelBase = ResizablePanel>
    class PyResizablePanel : public PyPanel<ResizablePanelBase>
    {
    public:
        using PyPanel<ResizablePanelBase>::PyPanel;

        void onEndResizing() override
        {
            PYBIND11_OVERRIDE(void, ResizablePanelBase, onEndResizing, );
        }
        void onStartResizing() override
        {
            PYBIND11_OVERRIDE(void, ResizablePanelBase, onStartResizing, );
        }
    };
}
