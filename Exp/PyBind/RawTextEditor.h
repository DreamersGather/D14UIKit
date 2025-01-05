#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/RawTextEditor.h"

namespace d14uikit
{
    void Py_InitRawTextEditor(py::module_& m);

    class ExRawTextEditor : public RawTextEditor
    {
    public:
        using RawTextEditor::onTextChange;
    };

    template<typename RawTextEditorBase = RawTextEditor>
    class PyRawTextEditor : public PyPanel<RawTextEditorBase>
    {
    public:
        using PyPanel<RawTextEditorBase>::PyPanel;

        void onTextChange(const std::wstring& text) override
        {
            PYBIND11_OVERRIDE(void, RawTextEditorBase, onTextChange, text);
        }
    };
}
