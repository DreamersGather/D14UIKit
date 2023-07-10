#pragma once

#include "Common/Precompile.h"

#include "UIKit/Window.h"

namespace d14engine::uikit
{
    struct MainWindow : Window
    {
        MainWindow(
            ShrdPtrParam<IconLabel> caption,
            float captionPanelHeight = 32.0f,
            float decorativeBarHeight = 4.0f);

        MainWindow(
            WstrParam title = L"Untitled",
            float captionPanelHeight = 32.0f,
            float decorativeBarHeight = 4.0f);

        void onInitializeFinish() override;

    protected:
        DisplayState m_originalDisplayState = {};

    public:
        void setDisplayState(DisplayState state);

    public:
        using CornerState = DWM_WINDOW_CORNER_PREFERENCE;

        constexpr static CornerState ROUND = DWMWCP_ROUND;
        constexpr static CornerState ROUNDSMALL = DWMWCP_ROUNDSMALL;
        constexpr static CornerState DONOTRUOND = DWMWCP_DONOTROUND;

    protected:
        CornerState m_cornerState = {};

    public:
        CornerState cornerState() const;
        void setCornerState(CornerState state);

    protected:
        // Window
        void onCloseHelper() override;

        void onRestoreHelper() override;

        void onMinimizeHelper() override;

        void onMaximizeHelper() override;
    };
}
