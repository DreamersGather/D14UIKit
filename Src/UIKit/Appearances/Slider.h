#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"

namespace d14engine::uikit::appearance
{
    struct Slider
    {
        struct Appearance : appearance::Appearance
        {
            Appearance();
            static void initialize();

            float thumbAreaExtension = 8.0f;

            struct Bar
            {
                struct Type
                {
                    struct Geometry
                    {
                        float height = {};
                        float roundRadius = {};
                    }
                    geometry = {};

                    SolidStyle background = {};
                    // i.e. background when disabled
                    SolidStyle secondaryBackground = {};
                }
                filled = {}, complete = {};
            }
            bar = {};

            struct Handle
            {
                struct Geometry
                {
                    D2D1_SIZE_F size = { 16.0f, 16.0f };
                    float roundRadius = 8.0f;
                }
                geometry = {};

                SolidStyle background = {};
                // i.e. background when disabled
                SolidStyle secondaryBackground = {};

                struct Shadow
                {
                    // Call loadHandleShadowBitmap after changing this field.
                    D2D1_RECT_F offset = { 2.0f, 2.0f, -2.0f, -2.0f };

                    D2D1_COLOR_F color = {};
                    // i.e. color when disabled
                    D2D1_COLOR_F secondaryColor = {};
                }
                shadow = {};
            }
            handle = {};

            struct ValueLabel
            {
                bool enabled = true;
                float offset = 12.0f;

                int precision = 0;
                bool isResident = false;

                struct Shadow
                {
                    D2D1_COLOR_F color = {};

                    float standardDeviation = 1.5f;
                }
                shadow = {};

                struct MainRect
                {
                    struct Geometry
                    {
                        // Call loadValueLabelMaskBitmap after changing this field.
                        // Call loadSideTrianglePathGeo after changing this field.
                        D2D1_SIZE_F size = { 35.0f, 21.0f };
                        float roundRadius = 5.0f;
                    }
                    geometry = {};

                    SolidStyle background = {};
                }
                mainRect = {};

                struct SideTriangle
                {
                    // Call loadValueLabelMaskBitmap after changing this field.
                    // Call loadSideTrianglePathGeo after changing this field.
                    D2D1_SIZE_F size = { 9.0f, 5.0f };

                    SolidStyle background = {};
                }
                sideTriangle = {};
            }
            valueLabel = {};

            struct ThemeData
            {
                struct Bar
                {
                    struct Type
                    {
                        SolidStyle background = {};
                        SolidStyle secondaryBackground = {};
                    }
                    filled = {}, complete = {};
                }
                bar = {};

                struct Handle
                {
                    SolidStyle background = {};
                    SolidStyle secondaryBackground = {};

                    struct Shadow
                    {
                        D2D1_COLOR_F color = {};
                        D2D1_COLOR_F secondaryColor = {};
                    }
                    shadow = {};
                }
                handle = {};

                struct ValueLabel
                {
                    struct MainRect
                    {
                        SolidStyle background = {};
                    }
                    mainRect = {};

                    struct SideTriangle
                    {
                        SolidStyle background = {};
                    }
                    sideTriangle = {};

                    struct Shadow
                    {
                        D2D1_COLOR_F color = {};
                    }
                    shadow = {};
                }
                valueLabel = {};
            };
            _D14_SET_THEME_DATA_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
