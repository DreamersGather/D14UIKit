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
        using PopupMenu::onTriggerMenuItem;
    };

    template<typename PopupMenuBase = PopupMenu>
    class PyPopupMenu : public PyScrollView<PopupMenuBase>
    {
    public:
        using PyScrollView::PyScrollView;

        void onTriggerMenuItem(const std::wstring& text) override
        {
            PYBIND11_OVERRIDE(void, PopupMenuBase, onTriggerMenuItem, text);
        }
    };
}
