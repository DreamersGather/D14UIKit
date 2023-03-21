#pragma once

#ifdef _D14_UIKIT_EXP_PY_BIND

#include "Common.h"

#include "Inc/Panel.h"

namespace d14uikit
{
    void Py_InitPanel(py::module_& m);

    class ExPanel : public Panel
    {
    public:
        using Panel::onSize;
        using Panel::onMove;
        using Panel::onChangeTheme;
        using Panel::onChangeLangLocale;
        using Panel::onGetFocus;
        using Panel::onLoseFocus;
        using Panel::onKeyboard;
        using Panel::onMouseEnter;
        using Panel::onMouseMove;
        using Panel::onMouseLeave;
        using Panel::onMouseWheel;
        using Panel::onMouseButton;
    };

    template<typename PanelBase = Panel>
    class PyPanel : public PanelBase
    {
    public:
        using PanelBase::PanelBase;

        void onSize(SizeEvent* e) override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onSize, e);
        }
        void onMove(MoveEvent* e) override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onMove, e);
        }
        void onChangeTheme(const std::wstring& name) override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onChangeTheme, name);
        }
        void onChangeLangLocale(const std::wstring& name) override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onChangeLangLocale, name);
        }
        void onGetFocus() override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onGetFocus, );
        }
        void onLoseFocus() override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onLoseFocus, );
        }
        void onKeyboard(KeyboardEvent* e) override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onKeyboard, e);
        }
        void onMouseEnter(MouseMoveEvent* e) override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onMouseEnter, e);
        }
        void onMouseMove(MouseMoveEvent* e) override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onMouseMove, e);
        }
        void onMouseLeave(MouseMoveEvent* e) override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onMouseLeave, e);
        }
        void onMouseWheel(MouseWheelEvent* e) override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onMouseWheel, e);
        }
        void onMouseButton(MouseButtonEvent* e) override
        {
            PYBIND11_OVERRIDE(void, PanelBase, onMouseButton, e);
        }
    };
}

#endif
