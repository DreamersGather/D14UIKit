#pragma once

#include "Panel.h"

_D14_UIKIT_FWDEF(ViewItem)

namespace d14uikit
{
    class IconLabel;

    class DllExport ViewItem : public Panel
    {
        _D14_UIKIT_PIMPL(ViewItem)

        explicit ViewItem(const std::wstring& text = L"ViewItem");

        IconLabel* content() const;

        const std::wstring& text() const;
        void setText(const std::wstring& text);
    };
}
