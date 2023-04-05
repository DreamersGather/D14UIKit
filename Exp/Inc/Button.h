#pragma once

#include "ClickablePanel.h"
#include "TextFormat.h"

namespace d14uikit
{
    class DllExport Button : public ClickablePanel
    {
        _D14_UIKIT_PIMPL(Button)

        explicit Button(const std::wstring& text = L"Button");

        Image* icon() const;
        void setIcon(Image* icon);

        Size iconSize() const;
        void setIconSize(const std::optional<Size>& value);

        const std::wstring& text() const;
        void setText(const std::wstring& text);

        _D14_UIKIT_TEXT_FORMAT_DECL_CONCRETE()
    };
}
