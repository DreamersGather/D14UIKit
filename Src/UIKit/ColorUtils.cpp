#include "Common/Precompile.h"

#include "UIKit/ColorUtils.h"

#include "Common/MathUtils/Basic.h"

namespace d14engine::uikit::color_utils
{
    iRGB convert(const D2D1_COLOR_F& rgb)
    {
        return
        {
            std::clamp(math_utils::round(rgb.r * 255.0f), 0, 255),
            std::clamp(math_utils::round(rgb.g * 255.0f), 0, 255),
            std::clamp(math_utils::round(rgb.b * 255.0f), 0, 255)
        };
    }

    D2D1_COLOR_F convert(const iRGB& rgb)
    {
        fRGB _rgb = {};
        _rgb.R = std::clamp((float)rgb.R / 255.0f, 0.0f, 1.0f);
        _rgb.G = std::clamp((float)rgb.G / 255.0f, 0.0f, 1.0f);
        _rgb.B = std::clamp((float)rgb.B / 255.0f, 0.0f, 1.0f);

        return D2D1::ColorF(_rgb.R, _rgb.G, _rgb.B);
    }

    iHSB rgb2hsb(const iRGB& rgb)
    {
        iHSB hsb = {};

        fRGB _rgb = {};
        fHSB _hsb = {};

        _rgb.R = std::clamp((float)rgb.R, 0.0f, 255.0f);
        _rgb.G = std::clamp((float)rgb.G, 0.0f, 255.0f);
        _rgb.B = std::clamp((float)rgb.B, 0.0f, 255.0f);

        auto minmax = std::minmax({ _rgb.R, _rgb.G, _rgb.B });
        auto& min = minmax.first;
        auto& max = minmax.second;

        // Calculate H
        if (min == max) _hsb.H = 0.0f;
        else if (max == _rgb.R)
        {
            if (_rgb.G >= _rgb.B)
            {
                _hsb.H = 60.0f * (_rgb.G - _rgb.B) / (max - min);
            }
            else // complements
            {
                _hsb.H = 60.0f * (_rgb.G - _rgb.B) / (max - min) + 360.0f;
            }
        }
        else if (max == _rgb.G)
        {
            _hsb.H = 60.0f * (_rgb.B - _rgb.R) / (max - min) + 120.0f;
        }
        else if (max == _rgb.B)
        {
            _hsb.H = 60.0f * (_rgb.R - _rgb.G) / (max - min) + 240.0f;
        }
        hsb.H = std::clamp(math_utils::round(_hsb.H), 0, 360);

        // Calculate S
        if (max == 0.0f)
        {
            _hsb.S = 0.0f;
        }
        else // valid denominator
        {
            _hsb.S = 100.0f * (max - min) / max;
        }
        hsb.S = std::clamp(math_utils::round(_hsb.S), 0, 100);

        // Calculate B
        hsb.B = std::clamp(math_utils::round(100.0f * max / 255.0f), 0, 100);

        return hsb;
    }

    iRGB hsb2rgb(const iHSB& hsb)
    {
        iRGB rgb = {};

        fRGB _rgb = {};
        fHSB _hsb = {};

        _hsb.H = std::clamp((float)hsb.H, 0.0f, 360.0f);
        _hsb.S = std::clamp((float)hsb.S / 100.0f, 0.0f, 1.0f);
        _hsb.B = std::clamp((float)hsb.B / 100.0f, 0.0f, 1.0f);

        // Calculate intermediates.
        auto N = _hsb.H / 60.0f;
        auto I = (int)N % 6;
        auto F = (N - I);
        auto P = _hsb.B * (1.0f - _hsb.S);
        auto Q = _hsb.B * (1.0f - F * _hsb.S);
        auto T = _hsb.B * (1.0f - (1.0f - F) * _hsb.S);

        // Decide RGB order by number.
        switch (I)
        {
        case 0: _rgb = { _hsb.B, T, P }; break;
        case 1: _rgb = { Q, _hsb.B, P }; break;
        case 2: _rgb = { P, _hsb.B, T }; break;
        case 3: _rgb = { P, Q, _hsb.B }; break;
        case 4: _rgb = { T, P, _hsb.B }; break;
        case 5: _rgb = { _hsb.B, P, Q }; break;
        default: break;
        }
        rgb.R = std::clamp(math_utils::round(_rgb.R * 255.0f), 0, 255);
        rgb.G = std::clamp(math_utils::round(_rgb.G * 255.0f), 0, 255);
        rgb.B = std::clamp(math_utils::round(_rgb.B * 255.0f), 0, 255);

        return rgb;
    }
}
