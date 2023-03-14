#pragma once

#include "Common.h"

#include "FilledButton.h"

namespace d14uikit
{
    class DllExport ElevatedButton : public FilledButton
    {
        _D14_UIKIT_PIMPL_DEF

        explicit ElevatedButton(const std::wstring& text = L"Button");

    protected:
        void initialize();
        explicit ElevatedButton(Passkey);
    };
}
