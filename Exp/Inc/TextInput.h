#pragma once

#include "RawTextInput.h"

_D14_UIKIT_FWDEF(TextInput)

namespace d14uikit
{
    class DllExport TextInput : public RawTextInput
    {
        _D14_UIKIT_PIMPL(TextInput)

        explicit TextInput(bool multiline);
    };
}
