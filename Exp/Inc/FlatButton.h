#pragma once

#include "Button.h"

namespace d14uikit
{
    class DllExport FlatButton : public Button
    {
        _D14_UIKIT_PIMPL(FlatButton)

        explicit FlatButton(const std::wstring& text = L"Button");
    };
}
