#include "Common/Precompile.h"

#include "UIKit/BitmapObject.h"

namespace d14engine::uikit
{
    bool BitmapObject::empty() const
    {
        if (bitmap != nullptr)
        {
            auto pixSize = bitmap->GetPixelSize();
            return pixSize.width == 0 || pixSize.height == 0;
        }
        else return true;
    }

    D2D1_INTERPOLATION_MODE BitmapObject::getInterpolationMode() const
    {
        D2D1_INTERPOLATION_MODE mode = {};
        if (interpolationMode.has_value())
        {
            mode = interpolationMode.value();
        }
        else mode = g_interpolationMode;
        
        // This mode uses a variable size high quality cubic kernel
        // to perform a pre-downscale the image if downscaling is involved
        // in the transform matrix, whichi not works for empty bitmap
        // in high DPI mode (Since that requests a invalid kernel size).
        if (mode == D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC && empty())
        {
            return D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
        }
        else return mode;
    }
    D2D1_INTERPOLATION_MODE
    BitmapObject::g_interpolationMode = D2D1_INTERPOLATION_MODE_LINEAR;
}
