#include "Common/Precompile.h"

#include "UIKit/ResourceUtils.h"

#include "Common/DirectXError.h"

#include "UIKit/Application.h"

namespace d14engine::uikit::resource_utils
{
    void initialize()
    {
        updateSystemFontNames();
        loadBasicSystemTextFormats();

        loadCommonBrushes();
        loadCommonEffects();
    }

    SystemFontNameSet g_systemFontNames = {};

    void updateSystemFontNames()
    {
        THROW_IF_NULL(Application::g_app);

        g_systemFontNames.clear();

        auto factory = Application::g_app->dx12Renderer()->dwriteFactory();

        ComPtr<IDWriteFontCollection1> fontCollection;
        THROW_IF_FAILED(factory->GetSystemFontCollection(FALSE, &fontCollection, TRUE));

        std::vector<ComPtr<IDWriteFontFamily>> fontFamilies(fontCollection->GetFontFamilyCount());

        for (UINT i = 0; i < fontFamilies.size(); ++i)
        {
            THROW_IF_FAILED(fontCollection->GetFontFamily(i, &fontFamilies[i]));
        }
        for (auto& family : fontFamilies)
        {
            ComPtr<IDWriteLocalizedStrings> familyNames;
            THROW_IF_FAILED(family->GetFamilyNames(&familyNames));

            UINT familyCount = familyNames->GetCount();
            for (UINT i = 0; i < familyCount; ++i)
            {
                UINT length = 0;

                THROW_IF_FAILED(familyNames->GetLocaleNameLength(i, &length));
                Wstring localeName(++length, L'\0');
                THROW_IF_FAILED(familyNames->GetLocaleName(i, localeName.data(), length));

                THROW_IF_FAILED(familyNames->GetStringLength(i, &length));
                Wstring fontName(++length, L'\0');
                THROW_IF_FAILED(familyNames->GetString(i, fontName.data(), length));

                g_systemFontNames.insert(
                {
                    fontName.erase(fontName.size() - 1),
                    localeName.erase(localeName.size() - 1)
                });
            }
        }
    }

    TextFormatMap g_textFormats = {};

    ComPtr<IDWriteTextFormat> loadSystemTextFormat(
        WstrParam textFormatName,
        WstrParam fontFamilyName,
        FLOAT fontSize,
        WstrParam localeName,
        DWRITE_FONT_WEIGHT fontWeight,
        DWRITE_FONT_STYLE fontStyle,
        DWRITE_FONT_STRETCH fontStretch)
    {
        THROW_IF_NULL(Application::g_app);

        THROW_IF_FAILED(Application::g_app->dx12Renderer()->dwriteFactory()->CreateTextFormat(
            fontFamilyName.c_str(),
            nullptr,
            fontWeight,
            fontStyle,
            fontStretch,
            // 1 inch == 72 pt == 96 dip
            fontSize * 96.0f / 72.0f,
            localeName.c_str(),
            &g_textFormats[textFormatName]));

        return g_textFormats[textFormatName];
    }

    void loadBasicSystemTextFormats()
    {
        for (int fontSize = 1; fontSize <= 100; ++fontSize)
        {
            loadSystemTextFormat
            (
                /* textFormatName */ L"默认/细体/" + std::to_wstring(fontSize),
                /* fontFamilyName */ L"微软雅黑",
                /* fontSize       */ (float)fontSize,
                /* localeName     */ L"zh-cn",
                /* fontWeight     */ DWRITE_FONT_WEIGHT_LIGHT
            );
            loadSystemTextFormat
            (
                /* textFormatName */ L"默认/正常/" + std::to_wstring(fontSize),
                /* fontFamilyName */ L"微软雅黑",
                /* fontSize       */ (float)fontSize,
                /* localeName     */ L"zh-cn",
                /* fontWeight     */ DWRITE_FONT_WEIGHT_NORMAL
            );
            loadSystemTextFormat
            (
                /* textFormatName */ L"默认/粗体/" + std::to_wstring(fontSize),
                /* fontFamilyName */ L"微软雅黑",
                /* fontSize       */ (float)fontSize,
                /* localeName     */ L"zh-cn",
                /* fontWeight     */ DWRITE_FONT_WEIGHT_BOLD
            );
            loadSystemTextFormat
            (
                /* textFormatName */ L"Default/Light/" + std::to_wstring(fontSize),
                /* fontFamilyName */ L"Segoe UI",
                /* fontSize       */ (float)fontSize,
                /* localeName     */ L"en-us",
                /* fontWeight     */ DWRITE_FONT_WEIGHT_LIGHT
            );
            loadSystemTextFormat
            (
                /* textFormatName */ L"Default/SemiLight/" + std::to_wstring(fontSize),
                /* fontFamilyName */ L"Segoe UI",
                /* fontSize       */ (float)fontSize,
                /* localeName     */ L"en-us",
                /* fontWeight     */ DWRITE_FONT_WEIGHT_SEMI_LIGHT
            );
            loadSystemTextFormat
            (
                /* textFormatName */ L"Default/Normal/" + std::to_wstring(fontSize),
                /* fontFamilyName */ L"Segoe UI",
                /* fontSize       */ (float)fontSize,
                /* localeName     */ L"en-us",
                /* fontWeight     */ DWRITE_FONT_WEIGHT_NORMAL
            );
            loadSystemTextFormat
            (
                /* textFormatName */ L"Default/SemiBold/" + std::to_wstring(fontSize),
                /* fontFamilyName */ L"Segoe UI",
                /* fontSize       */ (float)fontSize,
                /* localeName     */ L"en-us",
                /* fontWeight     */ DWRITE_FONT_WEIGHT_SEMI_BOLD
            );
            loadSystemTextFormat
            (
                /* textFormatName */ L"Default/Bold/" + std::to_wstring(fontSize),
                /* fontFamilyName */ L"Segoe UI",
                /* fontSize       */ (float)fontSize,
                /* localeName     */ L"en-us",
                /* fontWeight     */ DWRITE_FONT_WEIGHT_BOLD
            );
        }
    }

    const static String g_emptyStr = {};
    const String& emptyStrRef() { return g_emptyStr; }

    const static Wstring g_emptyWstr = {};
    const Wstring& emptyWstrRef() { return g_emptyWstr; }

    void loadCommonBrushes()
    {
        THROW_IF_NULL(Application::g_app);

        auto context = Application::g_app->dx12Renderer()->d2d1DeviceContext();

        // Solid Color Brush
        THROW_IF_FAILED(context->CreateSolidColorBrush(D2D1::ColorF{ 0x000000 }, &g_solidColorBrush));
    }

    ComPtr<ID2D1SolidColorBrush> g_solidColorBrush = {};

    void loadCommonEffects()
    {
        THROW_IF_NULL(Application::g_app);

        auto context = Application::g_app->dx12Renderer()->d2d1DeviceContext();

        // Shadow Effect
        THROW_IF_FAILED(context->CreateEffect(CLSID_D2D1Shadow, &g_shadowEffect));
    }

    ComPtr<ID2D1Effect> g_shadowEffect = {};

    Optional<Wstring> getClipboardText(HWND hWndNewOwner)
    {
        Optional<Wstring> content = std::nullopt;
        
        if (IsClipboardFormatAvailable(CF_UNICODETEXT) && OpenClipboard(hWndNewOwner))
        {
            auto hGlobal = GetClipboardData(CF_UNICODETEXT);
            if (hGlobal)
            {
                auto pGlobal = (WCHAR*)GlobalLock(hGlobal);
                if (pGlobal)
                {
                    content = pGlobal;
                    GlobalUnlock(hGlobal);
                }
            }
            CloseClipboard();
        }
        return content;
    }

    void setClipboardText(WstrParam content, HWND hWndNewOwner)
    {
        if (OpenClipboard(hWndNewOwner))
        {
            auto hGlobal = GlobalAlloc(GHND | GMEM_SHARE, sizeof(WCHAR) * (content.size() + 1));
            if (hGlobal)
            {
                auto pGlobal = (WCHAR*)GlobalLock(hGlobal);
                if (pGlobal)
                {
                    memcpy(pGlobal, content.data(), sizeof(WCHAR) * content.size());
                    GlobalUnlock(hGlobal);

                    if (EmptyClipboard()) SetClipboardData(CF_UNICODETEXT, hGlobal);
                }
            }
            CloseClipboard();
        }
    }

    KeyboardLayoutMap g_usKeyboardLayout =
    {
        // Alphabet
        { 0x41, { L'a', L'A' } },
        { 0x42, { L'b', L'B' } },
        { 0x43, { L'c', L'C' } },
        { 0x44, { L'd', L'D' } },
        { 0x45, { L'e', L'E' } },
        { 0x46, { L'f', L'F' } },
        { 0x47, { L'g', L'G' } },
        { 0x48, { L'h', L'H' } },
        { 0x49, { L'i', L'I' } },
        { 0x4A, { L'j', L'J' } },
        { 0x4B, { L'k', L'K' } },
        { 0x4C, { L'l', L'L' } },
        { 0x4D, { L'm', L'M' } },
        { 0x4E, { L'n', L'N' } },
        { 0x4F, { L'o', L'O' } },
        { 0x50, { L'p', L'P' } },
        { 0x51, { L'q', L'Q' } },
        { 0x52, { L'r', L'R' } },
        { 0x53, { L's', L'S' } },
        { 0x54, { L't', L'T' } },
        { 0x55, { L'u', L'U' } },
        { 0x56, { L'v', L'V' } },
        { 0x57, { L'w', L'W' } },
        { 0x58, { L'x', L'X' } },
        { 0x59, { L'y', L'Y' } },
        { 0x5A, { L'z', L'Z' } },
        // Numbers
        { 0x30, { L'0', L')' } },
        { 0x31, { L'1', L'!' } },
        { 0x32, { L'2', L'@' } },
        { 0x33, { L'3', L'#' } },
        { 0x34, { L'4', L'$' } },
        { 0x35, { L'5', L'%' } },
        { 0x36, { L'6', L'^' } },
        { 0x37, { L'7', L'&' } },
        { 0x38, { L'8', L'*' } },
        { 0x39, { L'9', L'(' } },
        // Symbols
        { VK_OEM_1, { L';', L':' } },
        { VK_OEM_2, { L'/', L'?' } },
        { VK_OEM_3, { L'`', L'~' } },
        { VK_OEM_4, { L'[', L'{' } },
        { VK_OEM_5, { L'\\', L'|' } },
        { VK_OEM_6, { L']', L'}' } },
        { VK_OEM_7, { L'\'', L'"' } },
        // Miscellaneous
        { VK_SPACE,      { L' ', L' ' } },
        { VK_OEM_PLUS,   { L'=', L'+' } },
        { VK_OEM_COMMA,  { L',', L'<' } },
        { VK_OEM_MINUS,  { L'-', L'_' } },
        { VK_OEM_PERIOD, { L'.', L'>' } },
        // NumPad Numbers
        { VK_NUMPAD0, { L'0', L'0' } },
        { VK_NUMPAD1, { L'1', L'1' } },
        { VK_NUMPAD2, { L'2', L'2' } },
        { VK_NUMPAD3, { L'3', L'3' } },
        { VK_NUMPAD4, { L'4', L'4' } },
        { VK_NUMPAD5, { L'5', L'5' } },
        { VK_NUMPAD6, { L'6', L'6' } },
        { VK_NUMPAD7, { L'7', L'7' } },
        { VK_NUMPAD8, { L'8', L'8' } },
        { VK_NUMPAD9, { L'9', L'9' } },
        // NumPad Symbols
        { VK_ADD,      { L'+', L'+' } },
        { VK_SUBTRACT, { L'-', L'-' } },
        { VK_MULTIPLY, { L'*', L'*' } },
        { VK_DIVIDE,   { L'/', L'/' } },
        { VK_DECIMAL,  { L'.', L'.' } },
    };
}
