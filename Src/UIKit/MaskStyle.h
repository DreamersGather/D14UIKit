#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit
{
    struct MaskStyle
    {
        MaskStyle(UINT bitmapWidth = 0, UINT bitmapHeight = 0);

        explicit MaskStyle(const D2D1_SIZE_U& size)
            :
            MaskStyle(size.width, size.height) { }

        D2D1_COLOR_F color = D2D1::ColorF{ 0x000000, 0.0f };
        float opacity = 1.0f;

        ComPtr<ID2D1Bitmap1> bitmap = {};

        void loadMaskBitmap(UINT width, UINT height);

        void loadMaskBitmap(const D2D1_SIZE_U& size);

        void beginMaskDraw(ID2D1DeviceContext* context, const D2D1_MATRIX_3X2_F& transform = D2D1::Matrix3x2F::Identity());

        void endMaskDraw(ID2D1DeviceContext* context);
    };
}
