#pragma once

#include "Panel.h"
#include "TextFormat.h"

namespace d14uikit
{
    class DllExport ViewItem : public Panel
    {
        _D14_UIKIT_PIMPL(ViewItem)

        explicit ViewItem(const std::wstring& text = L"ViewItem");

        _D14_UIKIT_TEXT_FORMAT_DECL

        Image* icon() const;
        void setIcon(Image* icon);

        Size iconSize() const;
        void setIconSize(const std::optional<Size>& value);

        const std::wstring& text() const;
        void setText(const std::wstring& text);
    };
}
