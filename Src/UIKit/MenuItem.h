#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/MenuItem.h"
#include "UIKit/ViewItem.h"

namespace d14engine::uikit
{
    struct MenuItem : appearance::MenuItem, ViewItem
    {
        friend struct PopupMenu;

        using ViewItem::ViewItem;

        _D14_SET_APPEARANCE_GETTER(MenuItem)

    protected:
        WeakPtr<PopupMenu> m_parentMenu = {};

        // Since the associated menu is created as a root UI object, it is
        // not destroyed after being disassociated with the menu-item, which
        // makes it convenient to transfer the menu between different items.
        // If you do not need the menu anymore, please call Panel::destory(),
        // otherwise the memory leaks.
        SharedPtr<PopupMenu> m_associatedMenu = {};

    public:
        const WeakPtr<PopupMenu>& parentMenu() const;

        WeakPtr<PopupMenu> associatedMenu() const;
        void setAssociatedMenu(ShrdPtrParam<PopupMenu> menu);

    public:
        // Controls whether to close parent-menus immediately after clicking.
        // It is useful when implementing something like checkable-menu-item.
        bool isInstant = true;

        void popupAssociatedMenu();

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        void onChangeThemeHelper(WstrParam themeName) override;

        void onMouseButtonHelper(MouseButtonEvent& e) override;

    public:
        void setEnabled(bool value) override;
    };
}
