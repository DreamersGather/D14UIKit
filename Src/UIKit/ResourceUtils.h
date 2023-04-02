#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit::resource_utils
{
    void initialize();

#pragma region Font

    struct SystemFontName { Wstring family, locale; };

    struct SystemFontNameSetLess
    {
        bool operator()(const SystemFontName& lhs, const SystemFontName& rhs) const
        {
            return std::tie(lhs.family, lhs.locale) < std::tie(rhs.family, rhs.locale);
        }
    };
    using SystemFontNameSet = std::set<SystemFontName, SystemFontNameSetLess>;

    extern SystemFontNameSet g_systemFontNames;

    using TextFormatMap = std::unordered_map<Wstring, ComPtr<IDWriteTextFormat>>;

    void updateSystemFontNames();

    extern TextFormatMap g_textFormats;

    void loadSystemTextFormat(
        WstrParam   textFormatName,     WstrParam   fontFamilyName,
        FLOAT       fontSize,           WstrParam   localeName,
        DWRITE_FONT_WEIGHT  fontWeight  = DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE   fontStyle   = DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

    void loadBasicSystemTextFormats();

#define D14_FONT(Key_Name) d14engine::uikit::resource_utils::g_textFormats.at(Key_Name).Get()

#pragma endregion

#pragma region Text

    const String& emptyStrRef();

    const Wstring& emptyWstrRef();

#pragma endregion

#pragma region Common

    void loadCommonBrushes();

    extern ComPtr<ID2D1SolidColorBrush> g_solidColorBrush;

    void loadCommonEffects();

    extern ComPtr<ID2D1Effect> g_shadowEffect;

#pragma endregion

#pragma region Clipboard

    Optional<Wstring> getClipboardText(HWND hWndNewOwner = nullptr);

    void setClipboardText(WstrParam content, HWND hWndNewOwner = nullptr);

#pragma endregion

#pragma region Keyboard Layout

    // Pick the alternative char if "shift" key pressed.
    struct PrintableChar { WCHAR normal, alternative; };

    using VirtualKeyCode = int;

    using KeyboardLayoutMap = std::unordered_map<VirtualKeyCode, PrintableChar>;

    extern KeyboardLayoutMap g_usKeyboardLayout; // US standard keyboard layout

#pragma endregion

}
