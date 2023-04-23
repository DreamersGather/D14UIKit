#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit
{
    struct BitmapObject
    {
        ComPtr<ID2D1Bitmap1> bitmap = {};

        bool empty() const;

        float opacity = 1.0f;

        static D2D1_INTERPOLATION_MODE g_interpolationMode;
        Optional<D2D1_INTERPOLATION_MODE> interpolationMode = std::nullopt;

        D2D1_INTERPOLATION_MODE getInterpolationMode() const;
    };
}
