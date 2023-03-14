#pragma once

#include "Common.h"

#include "FlatButton.h"

namespace d14uikit
{
    class DllExport OutlinedButton : public FlatButton
    {
        _D14_UIKIT_PIMPL_DEF

        explicit OutlinedButton(const std::wstring& text = L"Button");

    protected:
        void initialize();
        explicit OutlinedButton(Passkey);
    };
}
