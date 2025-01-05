#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/RawTextBox.h"

namespace d14uikit
{
    void Py_InitRawTextBox(py::module_& m);

    class ExRawTextBox : public RawTextBox
    {
    public:
        using RawTextBox::onTextChange;
    };

    template<typename RawTextBoxBase = RawTextBox>
    class PyRawTextBox : public PyPanel<RawTextBoxBase>
    {
    public:
        using PyPanel<RawTextBoxBase>::PyPanel;

        void onTextChange(const std::wstring& text) override
        {
            PYBIND11_OVERRIDE(void, RawTextBoxBase, onTextChange, text);
        }
    };
}
