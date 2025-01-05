#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Button.h"
#include "UIKit/ClickablePanel.h"

namespace d14engine::uikit
{
    struct IconLabel;

    struct Button : appearance::Button, ClickablePanel
    {
        Button(
            ShrdPtrParam<IconLabel> content,
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {});

        Button(
            WstrParam text = L"Button",
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {});

        enum class State { Idle, Hover, Down, Disabled, Count };

        void onInitializeFinish() override;

        _D14_SET_APPEARANCE_GETTER(Button)

        void setEnabled(bool value) override;

    protected:
        SharedPtr<IconLabel> m_content = {};

    public:
        const SharedPtr<IconLabel>& content() const;
        void setContent(ShrdPtrParam<IconLabel> content);

    protected:
        State m_currState = State::Idle;

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        bool isHitHelper(const uikit::Event::Point& p) const override;

        bool releaseUIObjectHelper(ShrdPtrParam<Panel> uiobj) override;

        void onSizeHelper(SizeEvent& e) override;

        void onChangeThemeHelper(WstrParam themeName) override;

        void onMouseEnterHelper(MouseMoveEvent& e) override;

        void onMouseLeaveHelper(MouseMoveEvent& e) override;

        // ClickablePanel
        void onMouseButtonPressHelper(ClickablePanel::Event& e) override;

        void onMouseButtonReleaseHelper(ClickablePanel::Event& e) override;
    };
}
