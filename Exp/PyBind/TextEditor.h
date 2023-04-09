#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/TextEditor.h"

namespace d14uikit
{
    void Py_InitTextEditor(py::module_& m);

    class ExTextEditor : public TextEditor
    {
    public:
        using TextEditor::onTextChange;
    };

    template<typename TextEditorBase = TextEditor>
    class PyTextEditor : public PyPanel<TextEditorBase>
    {
    public:
        using PyPanel::PyPanel;

        void onTextChange(const std::wstring& text) override
        {
            PYBIND11_OVERRIDE(void, TextEditorBase, onTextChange, text);
        }
    };
}
