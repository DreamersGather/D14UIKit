#pragma once

#include "FilledButton.h"

namespace d14uikit
{
    class DllExport ElevatedButton : public FilledButton
    {
        _D14_UIKIT_PIMPL(ElevatedButton)

        explicit ElevatedButton(const std::wstring& text = L"Button");
    };
}
