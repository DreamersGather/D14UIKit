#pragma once

#include "ViewItem.h"

namespace d14uikit
{
    class PopupMenu;

    class DllExport MenuItem : public ViewItem
    {
        _D14_UIKIT_PIMPL(MenuItem)

        MenuItem(
            const std::wstring& labelText = L"ViewItem",
            const std::wstring& hotkeyText = {});

        PopupMenu* associatedMenu() const;
        void setAssociatedMenu(PopupMenu* menu);

        const std::wstring& hotkeyText() const;
        void setHotkeyText(const std::wstring& text);

        void syncLabelHotkeyTextFormat();
    };
}
