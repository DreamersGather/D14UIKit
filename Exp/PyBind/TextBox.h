#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/TextBox.h"

namespace d14uikit
{
    void Py_InitTextBox(py::module_& m);

    class ExTextBox : public TextBox
    {
    public:
        using TextBox::onTextChange;
    };

    template<typename TextBoxBase = TextBox>
    class PyTextBox : public PyPanel<TextBoxBase>
    {
    public:
        using PyPanel<TextBoxBase>::PyPanel;

        void onTextChange(const std::wstring& text) override
        {
            PYBIND11_OVERRIDE(void, TextBoxBase, onTextChange, text);
        }
    };
}
