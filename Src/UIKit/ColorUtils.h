#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit::color_utils
{
    struct HEX;
    struct iRGB;
    struct fRGB;
    struct iHSB;
    struct fHSB;

    // #aabbggrr
    struct HEX
    {
        UINT32 abgr = {};

        constexpr HEX(COLORREF c) : abgr(c) {}

        HEX() = default;
        HEX(const iRGB& rgb);

        operator iRGB() const;
        explicit operator D2D1_COLOR_F() const;

        constexpr auto operator<=>(HEX rhs) const { return abgr <=> rhs.abgr; }
        constexpr bool operator==(HEX rhs) const { return abgr == rhs.abgr; }
    };

    // R: 0 ~ 255
    // G: 0 ~ 255
    // B: 0 ~ 255
    struct iRGB
    {
        int r = {}, g = {}, b = {};

        iRGB() = default;
        iRGB(int r, int g, int b);
        explicit iRGB(const D2D1_COLOR_F& c);

        operator fRGB() const;
        explicit operator iHSB() const;
        explicit operator fHSB() const;

        explicit operator D2D1_COLOR_F() const;
    };
    struct fRGB
    {
        float r = {}, g = {}, b = {};

        fRGB() = default;
        fRGB(float r, float g, float b);
        explicit fRGB(const D2D1_COLOR_F& c);

        operator iRGB() const;
        explicit operator iHSB() const;
        explicit operator fHSB() const;

        explicit operator D2D1_COLOR_F() const;
    };

    // H: 0 ~ 360
    // S: 0 ~ 100
    // B: 0 ~ 100
    struct iHSB
    {
        int h = {}, s = {}, b = {};

        iHSB() = default;
        iHSB(int h, int s, int b);
        explicit iHSB(const D2D1_COLOR_F& c);

        operator fHSB() const;
        explicit operator iRGB() const;
        explicit operator fRGB() const;

        explicit operator D2D1_COLOR_F() const;
    };
    struct fHSB
    {
        float h = {}, s = {}, b = {};

        fHSB() = default;
        fHSB(float h, float s, float b);
        explicit fHSB(const D2D1_COLOR_F& c);

        operator iHSB() const;
        explicit operator iRGB() const;
        explicit operator fRGB() const;

        explicit operator D2D1_COLOR_F() const;
    };

    iHSB rgb2hsb(const iRGB& rgb);
    iRGB hsb2rgb(const iHSB& hsb);
}
