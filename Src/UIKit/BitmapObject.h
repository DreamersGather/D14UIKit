#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit
{
    struct BitmapObject
    {
        BitmapObject() = default;

        // This constructor is intentionally declared as implicit
        // to facilitate the initialization from multiple bitmaps.
        BitmapObject(ComPtrParam<ID2D1Bitmap1> data);

        ComPtr<ID2D1Bitmap1> data = {};

        // Requires bitmap exists and size is greater than 0
        bool empty() const;

        float opacity = 1.0f;

        static D2D1_INTERPOLATION_MODE g_interpolationMode;
        Optional<D2D1_INTERPOLATION_MODE> interpolationMode = {};

        D2D1_INTERPOLATION_MODE getInterpolationMode() const;
    };
    using BmpObjParam = const BitmapObject&;
}
