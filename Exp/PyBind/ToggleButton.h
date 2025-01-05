#pragma once

#include "Common/Precompile.h"

#include "ClickablePanel.h"

#include "Inc/ToggleButton.h"

namespace d14uikit
{
    void Py_InitToggleButton(py::module_& m);

    class ExToggleButton : public ToggleButton
    {
    public:
        using ToggleButton::onStateChange;
    };

    template<typename ToggleButtonBase = ToggleButton>
    class PyToggleButton : public PyClickablePanel<ToggleButtonBase>
    {
    public:
        using PyClickablePanel<ToggleButtonBase>::PyClickablePanel;

        void onStateChange(ToggleButton::State state) override
        {
            PYBIND11_OVERRIDE(void, ToggleButtonBase, onStateChange, state);
        }
    };
}
