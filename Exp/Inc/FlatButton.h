#pragma once

#include "Button.h"

_D14_UIKIT_FWDEF(FlatButton)

namespace d14uikit
{
    class DllExport FlatButton : public Button
    {
        _D14_UIKIT_PIMPL(FlatButton)

        explicit FlatButton(const std::wstring& text = L"FlatButton");
    };
}
