#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/OnOffSwitch.h"
#include "UIKit/ClickablePanel.h"
#include "UIKit/StatefulObject.h"

namespace d14engine::uikit
{
    struct OnOffSwitch
        :
        appearance::OnOffSwitch, ClickablePanel,
        StatefulObject<OnOffSwitchState, OnOffSwitchStateChangeEvent>
    {
        OnOffSwitch(
            float roundRadius = 12.0f,
            const D2D1_RECT_F& rect = { 0.0f, 0.0f, 48.0f, 24.0f });

        _D14_SET_APPEARANCE_GETTER(OnOffSwitch)

        void setEnabled(bool value) override;

    public:
        constexpr static auto ON = State::ActiveFlag::On;
        constexpr static auto OFF = State::ActiveFlag::Off;

        void setOnOff(State::ActiveFlag flag);
        void setOnOffState(State::ActiveFlag flag);

    protected:
        void setOnOffWithAnim(State::ActiveFlag flag);

    protected:
        State::ActiveFlag m_animationTargetState = State::ActiveFlag::Finished;

        float m_currHandleLeftOffset = 0.0f;
        float m_currHandleDisplacement = 0.0f;

    protected:
        // IDrawObject2D
        void onRendererUpdateObject2DHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        void onChangeThemeHelper(WstrParam themeName) override;

        void onMouseEnterHelper(MouseMoveEvent& e) override;

        void onMouseLeaveHelper(MouseMoveEvent& e) override;

        // ClickablePanel
        void onMouseButtonPressHelper(ClickablePanel::Event& e) override;

        void onMouseButtonReleaseHelper(ClickablePanel::Event& e) override;
    };
}
