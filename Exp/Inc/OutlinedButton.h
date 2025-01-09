#pragma once

#include "FlatButton.h"

_D14_UIKIT_FWDEF(OutlinedButton)

namespace d14uikit
{
    class DllExport OutlinedButton : public FlatButton
    {
        _D14_UIKIT_PIMPL(OutlinedButton)

        explicit OutlinedButton(const std::wstring& text = L"OutlinedButton");
    };
}
