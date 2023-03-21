#pragma once

#ifdef _D14_UIKIT_EXP_PY_BIND

#include "Common.h"

#include "Panel.h"

#include "Inc/Window.h"

namespace d14uikit
{
    void Py_InitWindow(py::module_& m);

    class ExWindow : public d14uikit::Window
    {
    public:
        using Window::onClose;
        using Window::onRestore;
        using Window::onMaximize;
        using Window::onMinimize;
    };

    template<typename WindowBase = Window>
    class PyWindow : public PyPanel<WindowBase>
    {
    public:
        using PyPanel::PyPanel;

        void onEndDragging() override
        {
            PYBIND11_OVERRIDE(void, WindowBase, onEndDragging, );
        }
        void onStartDragging() override
        {
            PYBIND11_OVERRIDE(void, WindowBase, onStartDragging, );
        }
        void onEndResizing() override
        {
            PYBIND11_OVERRIDE(void, WindowBase, onEndResizing, );
        }
        void onStartResizing() override
        {
            PYBIND11_OVERRIDE(void, WindowBase, onStartResizing, );
        }
        void onClose() override
        {
            PYBIND11_OVERRIDE(void, WindowBase, onClose, );
        }
        void onRestore() override
        {
            PYBIND11_OVERRIDE(void, WindowBase, onRestore, );
        }
        void onMaximize() override
        {
            PYBIND11_OVERRIDE(void, WindowBase, onMaximize, );
        }
        void onMinimize() override
        {
            PYBIND11_OVERRIDE(void, WindowBase, onMinimize, );
        } 
    };
}

#endif
