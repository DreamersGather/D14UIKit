#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit::color_utils
{
    // R: 0 ~ 255
    // G: 0 ~ 255
    // B: 0 ~ 255
    struct iRGB { int R, G, B; };
    struct fRGB { float R, G, B; };
    // H: 0 ~ 360
    // S: 0 ~ 100
    // B: 0 ~ 100
    struct iHSB { int H, S, B; };
    struct fHSB { float H, S, B; };

    iRGB convert(const D2D1_COLOR_F& rgb);
    D2D1_COLOR_F convert(const iRGB& rgb);

    iHSB rgb2hsb(const iRGB& rgb);
    iRGB hsb2rgb(const iHSB& hsb);
}
