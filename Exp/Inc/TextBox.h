#pragma once

#include "LabelArea.h"
#include "TextInput.h"

namespace d14uikit
{
    class DllExport TextBox : public LabelArea
    {
        _D14_UIKIT_PIMPL(TextBox)
        _D14_UIKIT_TEXT_INPUT_DECL(TextBox)
    };
}
