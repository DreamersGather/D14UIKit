#pragma once

#include "Common/Precompile.h"

#include "ScrollView.h"

#include "Inc/PopupMenu.h"

namespace d14uikit
{
    void Py_InitPopupMenu(py::module_& m);

    class ExPopupMenu : public PopupMenu
    {
    public:
        using PopupMenu::onSelectChange;
        using PopupMenu::onTriggerMenuItem;
    };

    template<typename PopupMenuBase = PopupMenu>
    class PyPopupMenu : public PyScrollView<PopupMenuBase>
    {
    public:
        using PyScrollView<PopupMenuBase>::PyScrollView;

        void onSelectChange(const std::set<int>& selected) override
        {
            PYBIND11_OVERRIDE(void, PopupMenuBase, onSelectChange, selected);
        }
        void onTriggerMenuItem(const std::wstring& text) override
        {
            PYBIND11_OVERRIDE(void, PopupMenuBase, onTriggerMenuItem, text);
        }
    };
}
