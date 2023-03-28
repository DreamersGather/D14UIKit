#pragma once

#include "Common.h"

namespace d14uikit
{
    class DllExport Font
    {
        _D14_UIKIT_PIMPL_DEF

        explicit Font(const std::wstring& referName);

        Font(const Font& other);
        Font& operator=(const Font& rhs);

        std::wstring familyName() const;
        int size() const;
        std::wstring localeName() const;

        enum class Weight
        {
            Thin = 100,
            ExtraLight = 200,
            UltraLight = 200,
            Light = 300,
            SemiLight = 350,
            Normal = 400,
            Regular = 400,
            Medium = 500,
            DemiBold = 600,
            SemiBold = 600,
            Bold = 700,
            ExtraBold = 800,
            UltraBold = 800,
            Black = 900,
            Heavy = 900,
            ExtraBlack = 950,
            UltraBlack = 950
        };
        Weight weight() const;

#define SET_WEIGHT(Name) constexpr static auto Name = Weight::Name

        SET_WEIGHT(Thin);
        SET_WEIGHT(ExtraLight);
        SET_WEIGHT(Light);
        SET_WEIGHT(SemiLight);
        SET_WEIGHT(Regular);
        SET_WEIGHT(SemiBold);
        SET_WEIGHT(Bold);
        SET_WEIGHT(ExtraBold);
        SET_WEIGHT(Black);
        SET_WEIGHT(ExtraBlack);

#undef SET_WEIGHT

        enum class Style
        {
            Normal = 0,
            Oblique = 1,
            Italic = 2
        };
        Style style() const;

#define SET_STYLE(Name) constexpr static auto Name = Style::Name

        SET_STYLE(Normal);
        SET_STYLE(Oblique);
        SET_STYLE(Italic);

#undef SET_STYLE

        enum class Stretch
        {
            Undefined = 0,
            UltraCondensed = 1,
            ExtraCondensed = 2,
            Condensed = 3,
            SemiCondensed = 4,
            Normal = 5,
            Medium = 5,
            SemiExpanded = 6,
            Expanded = 7,
            ExtraExpanded = 8,
            UltraExpanded = 9
        };
        Stretch stretch() const;

#define SET_STRETCH(Name) constexpr static auto Name = Stretch::Name

        SET_STRETCH(ExtraCondensed);
        SET_STRETCH(Condensed);
        SET_STRETCH(SemiCondensed);
        SET_STRETCH(Medium);
        SET_STRETCH(SemiExpanded);
        SET_STRETCH(Expanded);
        SET_STRETCH(ExtraExpanded);

#undef SET_STRETCH

        // Returns "family/locale" paried-style strings.
        static std::set<std::wstring> querySystemFontNames();

        static bool load(
            const std::wstring& referName,
            const std::wstring& familyName,
            int size, // in point
            const std::wstring& localeName,
            Weight weight = Weight::Normal,
            Style style = Style::Normal,
            Stretch strech = Stretch::Normal);

    protected:
        void initialize();
        explicit Font(Passkey);
    };
}
