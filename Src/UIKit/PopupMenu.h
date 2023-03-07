#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/PopupMenu.h"
#include "UIKit/MenuItem.h"
#include "UIKit/ShadowStyle.h"
#include "UIKit/WaterfallView.h"

namespace d14engine::uikit
{
    // A popup menu should be created as a root UI object, so that it can
    // be activated at any position.  The design idea is that the menu is
    // used to perform some shortcuts and will be deactivated immediately
    // after use, so it is a bad idea to make the menu managed by another.

    struct PopupMenu : appearance::PopupMenu, WaterfallView<MenuItem>
    {
        explicit PopupMenu(const D2D1_RECT_F& rect = {});

        ShadowStyle shadow = {};

        D2D1_SIZE_F extendedSize(const D2D1_SIZE_F& rawSize) const;
        D2D1_SIZE_F narrowedSize(const D2D1_SIZE_F& rawSize) const;

        _D14_SET_APPEARANCE_GETTER(PopupMenu)

    public:
        void onChangeActivity(bool value);

        Function<void(PopupMenu*, bool)> f_onChangeActivity = {};

        using WaterfallView::ItemIndexParam;

        void onTriggerMenuItem(ItemIndexParam itemIndex);

        Function<void(PopupMenu*, ItemIndexParam)> f_onTriggerMenuItem = {};

    protected:
        virtual void onChangeActivityHelper(bool value);

        virtual void onTriggerMenuItemHelper(ItemIndexParam itemIndex);

    public:
        void insertItem(const ItemList& items, size_t index = 0) override;
        void appendItem(const ItemList& items);

        void removeItem(size_t index, size_t count = 1) override;

    protected:
        WeakPtr<MenuItem> m_associatedItem = {};

        friend void MenuItem::setAssociatedMenu(ShrdPtrParam<PopupMenu> menu);

    public:
        const WeakPtr<MenuItem>& associatedItem() const;

        void setActivated(bool value);

        void setActivatedIncludingParents(bool value);
        void setActivatedIncludingChildren(bool value);

    public:
        D2D1_RECT_F constrainedRectangle = math_utils::infiniteRectF();

        bool isHorzConstraintMeet(float expectedHorzOffset);
        bool isVertConstraintMeet(float expectedVertOffset);

        void showInConstrainedRect(D2D1_POINT_2F expectedOffset);

    protected:
        // Used by the top menu to close itself when clicking out of the area.
        SharedPtr<Panel> m_backgroundTriggerPanel = {};

    public:
        WeakPtr<Panel> backgroundTriggerPanel() const;
        void setBackgroundTriggerPanel(bool value);

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1LayerHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        void onSizeHelper(SizeEvent& e) override;

        void onChangeThemeHelper(WstrParam themeName) override;

        void onMouseMoveHelper(MouseMoveEvent& e) override;

        void onMouseLeaveHelper(MouseMoveEvent& e) override;

        void onMouseButtonHelper(MouseButtonEvent& e) override;
    };
}
