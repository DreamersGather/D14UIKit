#pragma once

#include "Button.h"

namespace d14uikit
{
    class DllExport FlatButton : public Button
    {
        _D14_UIKIT_PIMPL_DEF

        explicit FlatButton(const std::wstring& text = L"Button");

    protected:
        void initialize();
        explicit FlatButton(Passkey);
    };
}
