#pragma once

#include "Common/Precompile.h"

#include "UIKit/ColorUtils.h"
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
        // Helps set the correct state after restored from minimized.
        DisplayState m_originalDisplayState = {};

        /////////////////////////
        // Immersive Dark Mode //
        /////////////////////////
        //----------------------------------------------------------------
        // This setting is supported starting with Windows 11 Build 22000!
        //----------------------------------------------------------------

    protected:
        bool m_immersiveDarkMode = false;

    public:
        bool immersiveDarkMode() const;
        // Returns whether the setting was successful.
        bool setImmersiveDarkMode(bool value);

        //////////////////
        // Corner State //
        //////////////////
        //----------------------------------------------------------------
        // This setting is supported starting with Windows 11 Build 22000!
        //----------------------------------------------------------------

    public:
        using CornerState = DWM_WINDOW_CORNER_PREFERENCE;

        constexpr static CornerState Round = DWMWCP_ROUND;
        constexpr static CornerState RoundSmall = DWMWCP_ROUNDSMALL;
        constexpr static CornerState DoNotRound = DWMWCP_DONOTROUND;

    protected:
        CornerState m_cornerState = {};

        // Helps set the correct state after restored from maximized.
        CornerState m_originalCornerState = {};

    public:
        CornerState cornerState() const;
        // Returns whether the setting was successful.
        bool setCornerState(CornerState state);

        //////////////////
        // Border Color //
        //////////////////
        //----------------------------------------------------------------
        // This setting is supported starting with Windows 11 Build 22000!
        //----------------------------------------------------------------

    public:
        using BorderColor = color_utils::HEX;

        constexpr static BorderColor NoneColor = DWMWA_COLOR_NONE;
        constexpr static BorderColor DefaultColor = DWMWA_COLOR_DEFAULT;

    protected:
        BorderColor m_borderColor = {};

        bool setBorderColorAttr(BorderColor color);

        bool m_accentBorder = false;

    public:
        BorderColor borderColor() const;
        // Returns whether the setting was successful.
        bool setBorderColor(BorderColor color);

        bool accentBorder() const;
        bool setAccentBorder(bool value);

        //////////////////////
        // Special Material //
        //////////////////////
        //----------------------------------------------------------------
        // This setting is supported starting with Windows 11 Build 22621!
        //----------------------------------------------------------------

#define _D14_MAINWINDOW_MATERIAL_TYPE false

#if _D14_MAINWINDOW_MATERIAL_TYPE
    public:
        using MaterialType = DWM_SYSTEMBACKDROP_TYPE;

        constexpr static MaterialType NoneType = DWMSBT_NONE;
        constexpr static MaterialType AutoType = DWMSBT_AUTO;
        constexpr static MaterialType Mica = DWMSBT_MAINWINDOW;
        constexpr static MaterialType Acrylic = DWMSBT_TRANSIENTWINDOW;
        constexpr static MaterialType MicaAlt = DWMSBT_TABBEDWINDOW;

    protected:
        MaterialType m_materialType = AutoType;

    public:
        MaterialType materialType() const;
        // Returns whether the setting was successful.
        bool setMaterialType(MaterialType type);
#endif

    protected:
        // Panel
        void onChangeThemeStyleHelper(const ThemeStyle& style) override;

        // Window
        void onCloseHelper() override;

        void onRestoreHelper() override;

        void onMinimizeHelper() override;

        void onMaximizeHelper() override;
    };
}
