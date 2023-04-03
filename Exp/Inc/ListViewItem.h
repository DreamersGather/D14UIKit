#pragma once

#include "ViewItem.h"

namespace d14uikit
{
    class DllExport ListViewItem : public ViewItem
    {
        _D14_UIKIT_PIMPL_DEF

        explicit ListViewItem(const std::wstring& text = L"ViewItem");

    protected:
        void initialize();
        explicit ListViewItem(Passkey);
    };
}
