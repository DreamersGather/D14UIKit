#pragma once

#include "FlatButton.h"

namespace d14uikit
{
    class DllExport FilledButton : public FlatButton
    {
        _D14_UIKIT_PIMPL_DEF

        explicit FilledButton(const std::wstring& text = L"Button");

    protected:
        void initialize();
        explicit FilledButton(Passkey);
    };
}
