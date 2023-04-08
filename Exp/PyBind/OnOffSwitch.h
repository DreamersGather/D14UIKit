#pragma once

#include "Common/Precompile.h"

#include "ClickablePanel.h"

#include "Inc/OnOffSwitch.h"

namespace d14uikit
{
    void Py_InitOnOffSwitch(py::module_& m);

    class ExOnOffSwitch : public OnOffSwitch
    {
    public:
        using OnOffSwitch::onStateChange;
    };

    template<typename OnOffSwitchBase = OnOffSwitch>
    class PyOnOffSwitch : public PyClickablePanel<OnOffSwitchBase>
    {
    public:
        using PyClickablePanel::PyClickablePanel;

        void onStateChange(OnOffSwitch::State state) override
        {
            PYBIND11_OVERRIDE(void, OnOffSwitchBase, onStateChange, state);
        }
    };
}
