#pragma once

#include "Panel.h"

namespace d14uikit
{
    class DllExport ViewItem : public Panel
    {
        _D14_UIKIT_PIMPL_DEF

        explicit ViewItem(const std::wstring& text = L"ViewItem");

        Image* icon() const;
        void setIcon(Image* icon);

        Size iconSize() const;
        void setIconSize(const Size& value);

        bool customIconSize() const;
        void setCustomIconSize(bool value);

        const std::wstring& text() const;
        void setText(const std::wstring& text);

    protected:
        void initialize();
        explicit ViewItem(Passkey);
    };
}
