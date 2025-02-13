#pragma once

#include "Common/Precompile.h"

#include "UIKit/BitmapObject.h"

namespace d14engine::uikit
{
    struct MaskObject : BitmapObject
    {
        MaskObject(UINT bitmapWidth = 0, UINT bitmapHeight = 0);

        explicit MaskObject(const D2D1_SIZE_U& size)
            :
            MaskObject(size.width, size.height) { }

        D2D1_COLOR_F color = D2D1::ColorF{ 0x000000, 0.0f };

        void loadBitmap(UINT width, UINT height);

        void loadBitmap(const D2D1_SIZE_U& size);

        void beginDraw(ID2D1DeviceContext* context, const D2D1_MATRIX_3X2_F& transform = D2D1::Matrix3x2F::Identity());

        void endDraw(ID2D1DeviceContext* context);
    };
}
