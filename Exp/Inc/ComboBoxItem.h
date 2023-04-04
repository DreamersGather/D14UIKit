#pragma once

#include "MenuItem.h"

namespace d14uikit
{
    class DllExport ComboBoxItem : public MenuItem
    {
        _D14_UIKIT_PIMPL(ComboBoxItem)

        explicit ComboBoxItem(const std::wstring& text = L"ViewItem");
    };
}
