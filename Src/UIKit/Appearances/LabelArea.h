#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit::appearance
{
    struct LabelArea
    {
        struct Appearance : appearance::Appearance
        {
            Appearance();
            static void initialize();

            struct HiliteRange
            {
                SolidStyle background = {};
                StrokeStyle stroke = {};
            }
            hiliteRange = {};

            struct Indicator
            {
                bool visibility = true;

                SolidStyle background = {};

                struct Animation
                {
                    struct PeriodInSecs
                    {
                        float blink = 0.5f;
                    }
                    periodInSecs = {};
                }
                animation = {};
            }
            indicator = {};

            struct ThemeData
            {
                struct HiliteRange
                {
                    struct Background
                    {
                        D2D1_COLOR_F color = {};
                    }
                    background = {};
                }
                hiliteRange = {};

                struct Indicator
                {
                    struct Background
                    {
                        D2D1_COLOR_F color = {};
                    }
                    background = {};
                }
                indicator = {};
            };
            _D14_SET_THEME_DATA_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
