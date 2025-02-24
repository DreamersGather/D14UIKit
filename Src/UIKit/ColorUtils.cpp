#include "Common/Precompile.h"

#include "UIKit/ColorUtils.h"

#include "Common/MathUtils/Basic.h"

namespace d14engine::uikit::color_utils
{
    HEX::HEX(const iRGB& rgb)
        : abgr(RGB(rgb.r, rgb.g, rgb.b)) { }

    HEX::operator iRGB() const
    {
        return
        {
            (abgr & 0x00'00'00'ff) >> 0,
            (abgr & 0x00'00'ff'00) >> 8,
            (abgr & 0x00'ff'00'00) >> 16
        };
    }

    HEX::operator D2D1_COLOR_F() const
    {
        return
        {
            std::clamp((float)((abgr & 0x00'00'00'ff) >> 0) / 255.0f, 0.0f, 1.0f),
            std::clamp((float)((abgr & 0x00'00'ff'00) >> 8) / 255.0f, 0.0f, 1.0f),
            std::clamp((float)((abgr & 0x00'ff'00'00) >> 16) / 255.0f, 0.0f, 1.0f),
            std::clamp((float)((abgr & 0xff'00'00'00) >> 24) / 255.0f, 0.0f, 1.0f)
        };
    }

    iRGB::iRGB(int r, int g, int b)
        : r(r), g(g), b(b) { }

    iRGB::iRGB(const D2D1_COLOR_F& c)
        :
        r(std::clamp(math_utils::round(c.r * 255.0f), 0, 255)),
        g(std::clamp(math_utils::round(c.g * 255.0f), 0, 255)),
        b(std::clamp(math_utils::round(c.b * 255.0f), 0, 255)) { }

    iRGB::operator fRGB() const
    {
        return
        {
            std::clamp((float)r / 255.0f, 0.0f, 1.0f),
            std::clamp((float)g / 255.0f, 0.0f, 1.0f),
            std::clamp((float)b / 255.0f, 0.0f, 1.0f)
        };
    }

    iRGB::operator iHSB() const
    {
        return rgb2hsb(*this);
    }

    iRGB::operator fHSB() const
    {
        return rgb2hsb(*this);
    }

    iRGB::operator D2D1_COLOR_F() const
    {
        return
        {
            std::clamp((float)r / 255.0f, 0.0f, 1.0f),
            std::clamp((float)g / 255.0f, 0.0f, 1.0f),
            std::clamp((float)b / 255.0f, 0.0f, 1.0f),
            1.0f // alpha
        };
    }

    fRGB::fRGB(float r, float g, float b)
        : r(r), g(g), b(b) { }

    fRGB::fRGB(const D2D1_COLOR_F& c)
        : r(c.r), g(c.g), b(c.b) { }

    fRGB::operator iRGB() const
    {
        return
        {
            std::clamp(math_utils::round(r * 255.0f), 0, 255),
            std::clamp(math_utils::round(g * 255.0f), 0, 255),
            std::clamp(math_utils::round(b * 255.0f), 0, 255)
        };
    }

    fRGB::operator iHSB() const
    {
        return rgb2hsb(*this);
    }

    fRGB::operator fHSB() const
    {
        return rgb2hsb(*this);
    }

    fRGB::operator D2D1_COLOR_F() const
    {
        return { r, g, b, 1.0f };
    }

    iHSB::iHSB(int h, int s, int b)
        : h(h), s(s), b(b) { }

#pragma warning(push)
// Variable 'variable' is uninitialized. Always initialize a member variable.
#pragma warning(disable : 26495)

    iHSB::iHSB(const D2D1_COLOR_F& c)
        : iHSB((iRGB)c) { }

#pragma warning(pop)

    iHSB::operator fHSB() const
    {
        return
        {
            std::clamp((float)h / 360.0f, 0.0f, 1.0f),
            std::clamp((float)s / 100.0f, 0.0f, 1.0f),
            std::clamp((float)b / 100.0f, 0.0f, 1.0f)
        };
    }

    iHSB::operator iRGB() const
    {
        return hsb2rgb(*this);
    }

    iHSB::operator fRGB() const
    {
        return hsb2rgb(*this);
    }

    iHSB::operator D2D1_COLOR_F() const
    {
        return (D2D1_COLOR_F)hsb2rgb(*this);
    }

    fHSB::fHSB(float h, float s, float b)
        : h(h), s(s), b(b) { }

#pragma warning(push)
    // Variable 'variable' is uninitialized. Always initialize a member variable.
#pragma warning(disable : 26495)

    fHSB::fHSB(const D2D1_COLOR_F& c)
        : fHSB((iRGB)c) { }

#pragma warning(pop)

    fHSB::operator iHSB() const
    {
        return
        {
            std::clamp(math_utils::round(h * 360.0f), 0, 255),
            std::clamp(math_utils::round(s * 100.0f), 0, 255),
            std::clamp(math_utils::round(b * 100.0f), 0, 255)
        };
    }

    fHSB::operator iRGB() const
    {
        return hsb2rgb(*this);
    }

    fHSB::operator fRGB() const
    {
        return hsb2rgb(*this);
    }

    fHSB::operator D2D1_COLOR_F() const
    {
        return (D2D1_COLOR_F)hsb2rgb(*this);
    }

    iHSB rgb2hsb(const iRGB& rgb)
    {
        iHSB hsb = {};

        fRGB _rgb = {};
        fHSB _hsb = {};

        _rgb.r = std::clamp((float)rgb.r, 0.0f, 255.0f);
        _rgb.g = std::clamp((float)rgb.g, 0.0f, 255.0f);
        _rgb.b = std::clamp((float)rgb.b, 0.0f, 255.0f);

        auto minmax = std::minmax({ _rgb.r, _rgb.g, _rgb.b });
        auto& min = minmax.first;
        auto& max = minmax.second;

        // Calculate H
        if (min == max) _hsb.h = 0.0f;
        else if (max == _rgb.r)
        {
            if (_rgb.g >= _rgb.b)
            {
                _hsb.h = 60.0f * (_rgb.g - _rgb.b) / (max - min);
            }
            else // complements
            {
                _hsb.h = 60.0f * (_rgb.g - _rgb.b) / (max - min) + 360.0f;
            }
        }
        else if (max == _rgb.g)
        {
            _hsb.h = 60.0f * (_rgb.b - _rgb.r) / (max - min) + 120.0f;
        }
        else if (max == _rgb.b)
        {
            _hsb.h = 60.0f * (_rgb.r - _rgb.g) / (max - min) + 240.0f;
        }
        hsb.h = std::clamp(math_utils::round(_hsb.h), 0, 360);

        // Calculate S
        if (max == 0.0f)
        {
            _hsb.s = 0.0f;
        }
        else // valid denominator
        {
            _hsb.s = 100.0f * (max - min) / max;
        }
        hsb.s = std::clamp(math_utils::round(_hsb.s), 0, 100);

        // Calculate B
        hsb.b = std::clamp(math_utils::round(100.0f * max / 255.0f), 0, 100);

        return hsb;
    }

    iRGB hsb2rgb(const iHSB& hsb)
    {
        iRGB rgb = {};

        fRGB _rgb = {};
        fHSB _hsb = {};

        _hsb.h = std::clamp((float)hsb.h, 0.0f, 360.0f);
        _hsb.s = std::clamp((float)hsb.s / 100.0f, 0.0f, 1.0f);
        _hsb.b = std::clamp((float)hsb.b / 100.0f, 0.0f, 1.0f);

        // Calculate intermediates.
        auto n = _hsb.h / 60.0f;
        auto i = (int)n % 6;
        auto f = (n - i);
        auto p = _hsb.b * (1.0f - _hsb.s);
        auto q = _hsb.b * (1.0f - f * _hsb.s);
        auto t = _hsb.b * (1.0f - (1.0f - f) * _hsb.s);

        // Decide RGB order by number.
        switch (i)
        {
        case 0: _rgb = { _hsb.b, t, p }; break;
        case 1: _rgb = { q, _hsb.b, p }; break;
        case 2: _rgb = { p, _hsb.b, t }; break;
        case 3: _rgb = { p, q, _hsb.b }; break;
        case 4: _rgb = { t, p, _hsb.b }; break;
        case 5: _rgb = { _hsb.b, p, q }; break;
        default: break;
        }
        rgb.r = std::clamp(math_utils::round(_rgb.r * 255.0f), 0, 255);
        rgb.g = std::clamp(math_utils::round(_rgb.g * 255.0f), 0, 255);
        rgb.b = std::clamp(math_utils::round(_rgb.b * 255.0f), 0, 255);

        return rgb;
    }
}
