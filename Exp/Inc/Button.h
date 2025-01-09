#pragma once

#include "ClickablePanel.h"

_D14_UIKIT_FWDEF(Button)

namespace d14uikit
{
    class IconLabel;

    class DllExport Button : public ClickablePanel
    {
        _D14_UIKIT_PIMPL(Button)

        explicit Button(const std::wstring& text = L"Button");

        IconLabel* content() const;

        const std::wstring& text() const;
        void setText(const std::wstring& text);
    };
}
