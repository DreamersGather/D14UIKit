#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/Event.h"
#include "UIKit/SolidStyle.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit
{
    struct OnOffSwitchState
    {
        // Finished is not an actual state and only used to control animation.
        enum class ActiveFlag { On, Off, Count, Finished } activeFlag = {};
        enum class ButtonFlag { Idle, Hover, Down, Disabled, Count } buttonFlag = {};

        size_t index() const { return (size_t)buttonFlag + (size_t)activeFlag * (size_t)ButtonFlag::Count; }

        enum class Flag
        {
            OnIdle, OnHover, OnDown, OnDisabled,
            OffIdle, OffHover, OffDown, OffDisabled, Count
        };
    };

    struct OnOffSwitchStateChangeEvent : uikit::Event
    {
        OnOffSwitchState::ActiveFlag flag = {};

        bool operator==(const OnOffSwitchStateChangeEvent& other) const { return flag == other.flag; }

        bool on() const { return flag == OnOffSwitchState::ActiveFlag::On; }
        bool off() const { return flag == OnOffSwitchState::ActiveFlag::Off; }
    };
}

namespace d14engine::uikit::appearance
{
    struct OnOffSwitch
    {
        struct Appearance : appearance::Appearance
        {
            Appearance();
            static void initialize();

            struct Main
            {
                SolidStyle background = {};
                StrokeStyle stroke = {};
            }
            main[(size_t)OnOffSwitchState::Flag::Count] = {};

            struct Handle
            {
                struct Geometry
                {
                    D2D1_SIZE_F size = {};
                    float roundRadius = {};

                    Optional<float> leftOffset = {};
                    Optional<float> rightOffset = {};

                    float getLeftOffset(float parentWidth)
                    {
                        if (leftOffset.has_value())
                        {
                            return leftOffset.value();
                        }
                        else if (rightOffset.has_value())
                        {
                            return parentWidth - rightOffset.value() - size.width;
                        }
                        else return 0.0f; // return leftmost by default
                    }
                }
                geometry[(size_t)OnOffSwitchState::Flag::Count] = {};

                SolidStyle background[(size_t)OnOffSwitchState::Flag::Count] = {};

                struct Animation
                {
                    bool enabled = true;

                    struct DurationInSecs
                    {
                        float uniform = 0.05f;
                        float variable = 0.05f;
                    }
                    durationInSecs = {};
                }
                animation = {};
            }
            handle = {};

            struct ThemeStyle
            {
                Main main[(size_t)OnOffSwitchState::Flag::Count] = {};

                struct Handle
                {
                    SolidStyle background[(size_t)OnOffSwitchState::Flag::Count] = {};
                }
                handle = {};
            };
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
