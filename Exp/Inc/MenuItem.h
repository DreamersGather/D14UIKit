#pragma once

#include "ViewItem.h"

_D14_UIKIT_FWDEF(MenuItem)

namespace d14uikit
{
    class Label;

    class DllExport MenuItem : public ViewItem
    {
        friend class ComboBox;
        friend class PopupMenu;

        _D14_UIKIT_PIMPL(MenuItem)

        MenuItem(
            const std::wstring& labelText = L"MenuItem",
            const std::wstring& hotkeyText = {});

        Label* hotkey() const;

        // The return value may be null
        PopupMenu* associatedMenu() const;
        void setAssociatedMenu(PopupMenu* menu);

        bool instant() const;
        void setInstant(bool value);

        bool trigger() const;
        void setTrigger(bool value);
    };
}
