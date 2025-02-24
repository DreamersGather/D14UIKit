#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnumMagic.h"

#include "UIKit/Appearances/ToggleButton.h"
#include "UIKit/FilledButton.h"
#include "UIKit/StatefulObject.h"

namespace d14engine::uikit
{
    struct ToggleButtonState
    {
        enum class ActiveFlag { Activated, Deactivated } activeFlag = {};
        enum class ButtonFlag { Idle, Hover, Down, Disabled } buttonFlag = {};

        size_t index() const
        {
            return (size_t)buttonFlag + (size_t)activeFlag * cpp_lang_utils::enumCount<ButtonFlag>;
        }
        enum class Flag
        {
            ActivatedIdle, ActivatedHover, ActivatedDown, ActivatedDisabled,
            DeactivatedIdle, DeactivatedHover, DeactivatedDown, DeactivatedDisabled
        };
    };

    struct ToggleButtonStateChangeEvent : uikit::Event
    {
        ToggleButtonState::ActiveFlag flag = {};

        bool operator==(const ToggleButtonStateChangeEvent& rhs) const { return flag == rhs.flag; }

        bool activated() const { return flag == ToggleButtonState::ActiveFlag::Activated; }
        bool deactivated() const { return flag == ToggleButtonState::ActiveFlag::Deactivated; }
    };

    struct ToggleButton
        :
        appearance::ToggleButton, FilledButton,
        StatefulObject<ToggleButtonState, ToggleButtonStateChangeEvent>
    {
        ToggleButton(
            ShrdPtrParam<IconLabel> content,
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {});

        ToggleButton(
            WstrParam text = L"Button",
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {});

        _D14_SET_APPEARANCE_GETTER(ToggleButton)

    public:
        constexpr static auto ACTIVATED = StatefulObject::State::ActiveFlag::Activated;
        constexpr static auto DEACTIVATED = StatefulObject::State::ActiveFlag::Deactivated;

        void setActivated(StatefulObject::State::ActiveFlag flag);
        // Skips comparison and directly set the state.
        void setActivatedState(StatefulObject::State::ActiveFlag flag);

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        void onChangeThemeStyleHelper(const ThemeStyle& style) override;

        // ClickablePanel
        void onMouseButtonReleaseHelper(Button::Event& e) override;
    };
}
