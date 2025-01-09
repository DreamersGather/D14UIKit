#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/RawTextInput.h"

namespace d14uikit
{
    void Py_InitRawTextInput(py::module_& m);

    class ExRawTextInput : public RawTextInput
    {
    public:
        using RawTextInput::onTextChange;
    };

    template<typename RawTextInputBase = RawTextInput>
    class PyRawTextInput : public PyPanel<RawTextInputBase>
    {
    public:
        using PyPanel<RawTextInputBase>::PyPanel;

        void onTextChange(const std::wstring& text) override
        {
            PYBIND11_OVERRIDE(void, RawTextInputBase, onTextChange, text);
        }
    };
}
