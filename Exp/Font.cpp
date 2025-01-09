#include "Common/Precompile.h"

#include "Font.h"

#include "Common/DirectXError.h"
#include "Common/MathUtils/Basic.h"

#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    Font::Font(const std::wstring& name)
        :
        Font(Passkey{})
    {
        auto& formats = uikit::resource_utils::g_textFormats;
        auto formatItor = formats.find(name);
        if (formatItor != formats.end())
        {
            pimpl->textFormat = formatItor->second;
        }
        else // fall back to the default font
        {
            pimpl->textFormat = formats.at(L"Default/Normal/16");
        }
    }

    Font::Font(Passkey)
        :
        pimpl(std::make_shared<Impl>()) { }

    std::wstring Font::familyName() const
    {
        auto length = pimpl->textFormat->GetFontFamilyNameLength();
        std::wstring name(++length, L'\0');
        THROW_IF_FAILED(pimpl->textFormat->GetFontFamilyName(name.data(), length));
        return name;
    }

    int Font::size() const
    {
        return math_utils::round(pimpl->textFormat->GetFontSize() * 72.0f / 96.0f);
    }

    std::wstring Font::localeName() const
    {
        auto length = pimpl->textFormat->GetLocaleNameLength();
        std::wstring name(++length, L'\0');
        THROW_IF_FAILED(pimpl->textFormat->GetLocaleName(name.data(), length));
        return name;
    }

    Font::Weight Font::weight() const
    {
        return (Weight)pimpl->textFormat->GetFontWeight();
    }

    Font::Style Font::style() const
    {
        return (Style)pimpl->textFormat->GetFontStyle();
    }

    Font::Stretch Font::stretch() const
    {
        return (Stretch)pimpl->textFormat->GetFontStretch();
    }

    std::set<std::wstring> Font::querySystemFontNames()
    {
        std::set<std::wstring> dstNames = {};
        auto& srcNames = uikit::resource_utils::g_systemFontNames;
        std::transform(
            srcNames.begin(), srcNames.end(),
            std::inserter(dstNames, dstNames.begin()),
            [](const uikit::resource_utils::SystemFontName& elem)
            { return elem.family + L"/" + elem.locale; });
        return dstNames;
    }

    std::unique_ptr<Font> Font::load(
        const std::wstring& name,
        const std::wstring& familyName,
        int size,
        const std::wstring& localeName,
        Weight weight,
        Style style,
        Stretch strech)
    {
        uikit::resource_utils::loadSystemTextFormat
        (
            /* textFormatName */ name,
            /* fontFamilyName */ familyName,
            /* fontSize       */ (float)size,
            /* localeName     */ localeName,
            /* fontWeight     */ (DWRITE_FONT_WEIGHT)weight,
            /* fontStyle      */ (DWRITE_FONT_STYLE)style,
            /* fontStretch    */ (DWRITE_FONT_STRETCH)strech
        );
        return std::make_unique<Font>(name);
    }
}
