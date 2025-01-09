#pragma once

#include "Label.h"

_D14_UIKIT_FWDEF(LabelArea)

namespace d14uikit
{
    class DllExport LabelArea : public Label
    {
        _D14_UIKIT_PIMPL(LabelArea)

        explicit LabelArea(const std::wstring& text = {});

        int caretOffset() const;
        void setCaretOffset(int value);

        Range hiliteRange() const;
        void setHiliteRange(const Range& value);

        void performCommandCtrlA();
        void performCommandCtrlC();
    };
}
