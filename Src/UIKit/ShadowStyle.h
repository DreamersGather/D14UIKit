#pragma once

#include "Common/Precompile.h"

#include "UIKit/BitmapObject.h"

namespace d14engine::uikit
{
    struct ShadowStyle : BitmapObject
    {
        ShadowStyle(UINT bitmapWidth = 0, UINT bitmapHeight = 0);

        explicit ShadowStyle(const D2D1_SIZE_U& size)
            :
            ShadowStyle(size.width, size.height) { }

        D2D1_COLOR_F color = D2D1::ColorF{ 0x000000 };

        float standardDeviation = 3.0f;

        D2D1_SHADOW_OPTIMIZATION optimization = D2D1_SHADOW_OPTIMIZATION_BALANCED;

        void loadBitmap(UINT width, UINT height);

        void loadBitmap(const D2D1_SIZE_U& size);

        void beginDraw(ID2D1DeviceContext* context, const D2D1_MATRIX_3X2_F& transform = D2D1::Matrix3x2F::Identity());

        void endDraw(ID2D1DeviceContext* context);

        void configEffectInput(ID2D1Effect* effect);
    };
}
