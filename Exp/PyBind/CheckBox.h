#pragma once

#include "Common/Precompile.h"

#include "ClickablePanel.h"

#include "Inc/CheckBox.h"

namespace d14uikit
{
    void Py_InitCheckBox(py::module_& m);

    class ExCheckBox : public CheckBox
    {
    public:
        using CheckBox::onStateChange;
    };

    template<typename CheckBoxBase = CheckBox>
    class PyCheckBox : public PyClickablePanel<CheckBoxBase>
    {
    public:
        using PyClickablePanel<CheckBoxBase>::PyClickablePanel;

        void onStateChange(CheckBox::State state) override
        {
            PYBIND11_OVERRIDE(void, CheckBoxBase, onStateChange, state);
        }
    };
}
