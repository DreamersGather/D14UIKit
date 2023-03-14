#pragma once

#include "Common.h"

#include "ClickablePanel.h"

namespace d14uikit
{
    class DllExport Button : public ClickablePanel
    {
        _D14_UIKIT_PIMPL_DEF

        explicit Button(const std::wstring& text = L"Button");

        Image* icon() const;
        void setIcon(Image* icon);

        Size iconSize() const;
        void setIconSize(bool value);
        void setIconSize(const Size& value);

        const std::wstring& text() const;
        void setText(const std::wstring& text);

    protected:
        void initialize();
        explicit Button(Passkey);
    };
}
