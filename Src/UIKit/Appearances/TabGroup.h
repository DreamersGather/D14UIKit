#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/Appearances/PopupMenu.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit::appearance
{
    struct TabGroup
    {
        enum class CardState { Active, Hover, Dormant, Count };

        enum class ButtonState { Idle, Hover, Down, Count };

        struct Appearance : appearance::Appearance
        {
            Appearance();
            static void initialize();

            SolidStyle background = {};
            StrokeStyle stroke = {};

            struct TabBar
            {
                struct Geometry
                {
                    float height = 32.0f;
                    float rightPadding = 30.0f;
                }
                geometry = {};

                // Call updateCandidateTabInfo/updatePreviewPanelItems
                // after the card-geometry-environment changes.
                struct Card
                {
                    struct Main
                    {
                        struct Geometry
                        {
                            // Call loadActiveCardShadowBitmap after changing size of active-card.
                            D2D1_SIZE_F size = {};
                            float roundRadius = {};
                        }
                        geometry = {};

                        SolidStyle background = {};
                    }
                    main[(size_t)CardState::Count] = {};

                    D2D1_COLOR_F activeShadowColor = {};
                }
                card = {};

                struct Separator
                {
                    struct Geometry
                    {
                        D2D1_SIZE_F size = { 1.0f, 16.0f };
                        D2D1_POINT_2F offset = { 0.0f, 4.0f };
                    }
                    geometry = {};

                    SolidStyle background = { D2D1::ColorF{ 0x808080 }, 1.0f };
                }
                separator = {};

                struct MoreCards
                {
                    struct Control
                    {
                        struct Icon
                        {
                            struct Geometry
                            {
                                struct TopRect
                                {
                                    D2D1_SIZE_F size = { 10.0f, 2.0f };
                                    D2D1_POINT_2F offset = { 4.0f, 4.0f };
                                }
                                topRect = {};

                                struct BottomTriangle
                                {
                                    D2D1_POINT_2F point0 = { 4.0f, 8.0f };
                                    D2D1_POINT_2F point1 = { 14.0f, 8.0f };
                                    D2D1_POINT_2F point2 = { 9.0f, 14.0f };
                                }
                                bottomTriangle = {};
                            }
                            geometry = {};

                            SolidStyle background[(size_t)ButtonState::Count] = {};
                        }
                        icon = {};

                        struct Button
                        {
                            struct Geometry
                            {
                                D2D1_SIZE_F size = { 18.0f, 18.0f };
                                D2D1_POINT_2F offset = { -24.0f, 3.0f };

                                float roundRadius = 4.0f;
                            }
                            geometry = {};

                            SolidStyle background[(size_t)ButtonState::Count] = {};
                        }
                        button = {};
                    }
                    control = {};

                    struct PreviewPanel
                    {
                        float itemHeight = 30.0f;
                        D2D1_POINT_2F offset = { 0.0f, 5.0f };

                        PopupMenu::Appearance::Geometry geometry =
                        {
                            .extension = 5.0f,
                            .roundRadius = 5.0f
                        };
                        PopupMenu::Appearance::Shadow shadow =
                        {
                            .offset = { 1.0f, 2.0f, -1.0f, 0.0f },
                            .standardDeviation = 3.0f
                        };
                    }
                    previewPanel = {};
                }
                moreCards = {};
            }
            tabBar = {};

            SolidStyle maskWhenBelowDragWindow = {};

            struct ThemeStyle
            {
                struct Background
                {
                    D2D1_COLOR_F color = {};
                }
                background = {};

                struct Stroke
                {
                    D2D1_COLOR_F color = {};
                }
                stroke = {};

                struct TabBar
                {
                    struct Card
                    {
                        struct Main
                        {
                            SolidStyle background = {};
                        }
                        main[(size_t)CardState::Count] = {};

                        D2D1_COLOR_F activeShadowColor = {};
                    }
                    card = {};

                    struct MoreCards
                    {
                        struct Control
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
                        control = {};
                    }
                    moreCards = {};
                }
                tabBar = {};

                SolidStyle maskWhenBelowDragWindow = {};
            };
            _D14_SET_THEME_STYLE_MAP_DECL;

            void changeTheme(WstrParam themeName) override;
        }
        appearance = {};
    };
}
