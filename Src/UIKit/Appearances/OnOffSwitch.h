#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnumMagic.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/Event.h"
#include "UIKit/SolidStyle.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit
{
    struct OnOffSwitchState
    {
        // Finished is not an actual state and only used to control animation.
        enum class ActiveFlag { On, Off, Finished } activeFlag = {};
        enum class ButtonFlag { Idle, Hover, Down, Disabled } buttonFlag = {};

        size_t index() const
        {
            return (size_t)buttonFlag + (size_t)activeFlag * cpp_lang_utils::enumCount<ButtonFlag>;
        }
        enum class Flag
        {
            OnIdle, OnHover, OnDown, OnDisabled,
            OffIdle, OffHover, OffDown, OffDisabled
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
            main[cpp_lang_utils::enumCount<OnOffSwitchState::Flag>] = {};

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
                geometry[cpp_lang_utils::enumCount<OnOffSwitchState::Flag>] = {};

                SolidStyle background[cpp_lang_utils::enumCount<OnOffSwitchState::Flag>] = {};

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

            struct ThemeData
            {
                Main main[cpp_lang_utils::enumCount<OnOffSwitchState::Flag>] = {};

                struct Handle
                {
                    SolidStyle background[cpp_lang_utils::enumCount<OnOffSwitchState::Flag>] = {};
                }
                handle = {};
            };
            _D14_SET_THEME_DATA_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
