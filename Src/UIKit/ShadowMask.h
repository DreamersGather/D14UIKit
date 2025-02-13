#pragma once

#include "Common/Precompile.h"

#include "UIKit/MaskObject.h"

namespace d14engine::uikit
{
    struct ShadowMask : MaskObject
    {
        using MaskObject::MaskObject;

        D2D1_COLOR_F color = D2D1::ColorF{ 0x000000 };

        float standardDeviation = 3.0f;

        D2D1_SHADOW_OPTIMIZATION optimization = D2D1_SHADOW_OPTIMIZATION_BALANCED;

        void configEffectInput(ID2D1Effect* effect);
    };
}
