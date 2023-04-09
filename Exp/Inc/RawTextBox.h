#pragma once

#include "LabelArea.h"
#include "RawTextInput.h"

namespace d14uikit
{
    class DllExport RawTextBox : public LabelArea
    {
        _D14_UIKIT_PIMPL(RawTextBox)
        _D14_UIKIT_RAW_TEXT_INPUT_DECL(RawTextBox)
    };
}
