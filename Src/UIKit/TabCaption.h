#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/TabCaption.h"
#include "UIKit/Panel.h"

namespace d14engine::uikit
{
    struct IconLabel;

    struct TabCaption : appearance::TabCaption, Panel
    {
        friend struct TabGroup;

        // Intentionally declared as implicit to be compatible with IconLabel.
        TabCaption(ShrdPtrParam<IconLabel> title);

        explicit TabCaption(WstrParam text = L"Untitled");

        void onInitializeFinish() override;

        _D14_SET_APPEARANCE_GETTER(TabCaption)

    protected:
        SharedPtr<IconLabel> m_title = {};

        WeakPtr<TabGroup> m_parentTabGroup = {};

    public:
        const SharedPtr<IconLabel>& title() const;
        void setTitle(ShrdPtrParam<IconLabel> title);

        const WeakPtr<TabGroup>& parentTabGroup() const;

    protected:
        D2D1_RECT_F titleSelfcoordRect() const;

        D2D1_RECT_F closeIconAbsoluteRect(const D2D1_RECT_F& buttonRect) const;
        D2D1_RECT_F closeButtonAbsoluteRect() const;

    public:
        bool closable = true, draggable = true, promotable = false;

    protected:
        bool m_isCloseButtonHover = false;
        bool m_isCloseButtonDown = false;

        ButtonState getCloseButtonState() const;

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        bool isHitHelper(const Event::Point& p) const override;

        void onSizeHelper(SizeEvent& e) override;

        void onChangeThemeHelper(WstrParam themeName) override;

        void onMouseMoveHelper(MouseMoveEvent& e) override;

        void onMouseLeaveHelper(MouseMoveEvent& e) override;

        void onMouseButtonHelper(MouseButtonEvent& e) override;
    };
}
