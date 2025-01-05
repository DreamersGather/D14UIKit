#pragma once

#include "Common/Precompile.h"

#include "ClickablePanel.h"

#include "Inc/ComboBox.h"

namespace d14uikit
{
    void Py_InitComboBox(py::module_& m);

    class ExComboBox : public ComboBox
    {
    public:
        using ComboBox::onSelectedChange;
    };

    template<typename ComboBoxBase = ComboBox>
    class PyComboBox : public PyClickablePanel<ComboBoxBase>
    {
    public:
        using PyClickablePanel<ComboBoxBase>::PyClickablePanel;

        void onSelectedChange(const std::wstring& text) override
        {
            PYBIND11_OVERRIDE(void, ComboBoxBase, onSelectedChange, text);
        }
    };
}
