#pragma once

#include "LabelArea.h"
#include "TextInput.h"

namespace d14uikit
{
    class DllExport TextEditor : public LabelArea
    {
        _D14_UIKIT_PIMPL(TextEditor)
        _D14_UIKIT_TEXT_INPUT_DECL(TextEditor)
    };
}
