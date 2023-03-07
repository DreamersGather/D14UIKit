#include "Common/Precompile.h"

#include "UIKit/ShadowStyle.h"

#include "Common/DirectXError.h"
#include "UIKit/Application.h"
#include "UIKit/BitmapUtils.h"
#include "UIKit/ResourceUtils.h"

namespace d14engine::uikit
{
    ShadowStyle::ShadowStyle(UINT bitmapWidth, UINT bitmapHeight)
    {
        loadShadowBitmap(bitmapWidth, bitmapHeight);
    }

    void ShadowStyle::loadShadowBitmap(UINT width, UINT height)
    {
        resource_utils::g_shadowEffect->SetInput(0, nullptr);

        auto rndr = Application::g_app->dxRenderer();
        rndr->beginGpuCommand();

        bitmap = bitmap_utils::loadBitmap(
            width, height, nullptr, D2D1_BITMAP_OPTIONS_TARGET);

        rndr->endGpuCommand();
    }

    void ShadowStyle::loadShadowBitmap(const D2D1_SIZE_U& size)
    {
        return loadShadowBitmap(size.width, size.height);
    }

    void ShadowStyle::beginShadowDraw(ID2D1DeviceContext* context, const D2D1_MATRIX_3X2_F& transform)
    {
        // It is recommended to call SetTarget before BeginDraw.
        // The program may crash if the previous target is a synchronized
        // resource and still bound to the context when calling BeginDraw.
        context->SetTarget(bitmap.Get());

        context->BeginDraw();
        context->SetTransform(transform);
        context->Clear(D2D1::ColorF{ 0x000000, 0.0f });
    }

    void ShadowStyle::endShadowDraw(ID2D1DeviceContext* context)
    {
        THROW_IF_FAILED(context->EndDraw());
    }

    void ShadowStyle::configShadowEffectInput(ID2D1Effect* effect)
    {
        effect->SetInput(0, bitmap.Get());

        THROW_IF_FAILED(effect->SetValue(D2D1_SHADOW_PROP_COLOR, color));
        THROW_IF_FAILED(effect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, standardDeviation));
        THROW_IF_FAILED(effect->SetValue(D2D1_SHADOW_PROP_OPTIMIZATION, optimization));
    }
}
