#pragma once

#include "FlatButton.h"

_D14_UIKIT_FWDEF(FilledButton)

namespace d14uikit
{
    class DllExport FilledButton : public FlatButton
    {
        _D14_UIKIT_PIMPL(FilledButton)

        explicit FilledButton(const std::wstring& text = L"FilledButton");
    };
}
