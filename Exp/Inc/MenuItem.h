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

        bool instant() const;
        void setInstant(bool value);

        bool trigger() const;
        void setTrigger(bool value);

        const std::wstring& hotkeyText() const;
        void setHotkeyText(const std::wstring& text);

        _D14_UIKIT_TEXT_FORMAT_DECL_OPTIONAL(Hotkey)
    };
}
