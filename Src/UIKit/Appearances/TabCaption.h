#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"

namespace d14engine::uikit::appearance
{
    struct TabCaption
    {
        enum class ButtonState { Idle, Hover, Down, Count };

        struct Appearance : appearance::Appearance
        {
            static void initialize();

            struct Title
            {
                float leftPadding = 12.0f;
                float rightPadding = 32.0f;
            }
            title = {};

            struct CloseX
            {
                struct Icon
                {
                    struct Geometry
                    {
                        D2D1_SIZE_F size = { 8.0f, 8.0f };
                        D2D1_POINT_2F offset = { 4.0f, 4.0f };
                    }
                    geometry = {};

                    SolidStyle background[(size_t)ButtonState::Count] = {};

                    float strokeWidth = 1.0f;
                }
                icon = {};

                struct Button
                {
                    struct Geometry
                    {
                        D2D1_SIZE_F size = { 16.0f, 16.0f };
                        float rightOffset = -24.0f; // vertical center

                        float roundRadius = 4.0f;
                    }
                    geometry = {};

                    SolidStyle background[(size_t)ButtonState::Count] = {};
                }
                button = {};
            }
            closeX = {};

            struct ThemeStyle
            {
                struct CloseX
                {
                    struct Icon
                    {
                        SolidStyle background[(size_t)ButtonState::Count] = {};
                    }
                    icon = {};

                    struct Button
                    {
                        SolidStyle background[(size_t)ButtonState::Count] = {};
                    }
                    button = {};
                }
                closeX = {};
            };
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
