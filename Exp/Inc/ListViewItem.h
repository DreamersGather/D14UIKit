#pragma once

#include "ViewItem.h"

_D14_UIKIT_FWDEF(ListViewItem)

namespace d14uikit
{
    class DllExport ListViewItem : public ViewItem
    {
        friend class ListView;

        _D14_UIKIT_PIMPL(ListViewItem)

        explicit ListViewItem(const std::wstring& text = L"ListViewItem");
    };
}
