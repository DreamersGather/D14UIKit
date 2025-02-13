#include "Common/Precompile.h"

#include "UIKit/ShadowMask.h"

#include "Common/DirectXError.h"

namespace d14engine::uikit
{
    void ShadowMask::configEffectInput(ID2D1Effect* effect)
    {
        effect->SetInput(0, data.Get());

        THROW_IF_FAILED(effect->SetValue(D2D1_SHADOW_PROP_COLOR, color));
        THROW_IF_FAILED(effect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, standardDeviation));
        THROW_IF_FAILED(effect->SetValue(D2D1_SHADOW_PROP_OPTIMIZATION, optimization));
    }
}
