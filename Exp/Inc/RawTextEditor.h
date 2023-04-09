#pragma once

#include "LabelArea.h"
#include "RawTextInput.h"

namespace d14uikit
{
    class DllExport RawTextEditor : public LabelArea
    {
        _D14_UIKIT_PIMPL(RawTextEditor)
        _D14_UIKIT_RAW_TEXT_INPUT_DECL(RawTextEditor)
    };
}
