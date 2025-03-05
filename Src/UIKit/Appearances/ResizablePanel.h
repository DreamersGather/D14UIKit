#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/SolidStyle.h"

namespace d14engine::uikit::appearance
{
    struct ResizablePanel
    {
        struct Appearance : appearance::Appearance
        {
            static void initialize();

            struct SizingFrame
            {
                D2D1_RECT_F extension = { 6.0f, 6.0f, 6.0f, 6.0f };
                D2D1_RECT_F cornerOffset = { 12.0f, 12.0f, 12.0f, 12.0f };
            }
            sizingFrame = {};

            struct StaticSizingGuideFrame
            {
                SolidStyle background = {};

                float strokeWidth = 6.0f;
            }
            staticSizingGuideFrame = {};

            struct ThemeData
            {
                StaticSizingGuideFrame staticSizingGuideFrame = {};
            };
            _D14_SET_THEME_DATA_MAP_DECL;

            void changeTheme(WstrParam themeName);
        }
        appearance = {};
    };
}
