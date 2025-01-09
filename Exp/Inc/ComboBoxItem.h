#pragma once

#include "MenuItem.h"

namespace d14uikit
{
    class DllExport ComboBoxItem : public MenuItem
    {
    public:
        explicit ComboBoxItem(const std::wstring& text = L"ComboBoxItem");
    };
}
