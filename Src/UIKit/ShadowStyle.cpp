#include "Common/Precompile.h"

#include "UIKit/ShadowStyle.h"

#include "Common/DirectXError.h"

#include "UIKit/Application.h"
#include "UIKit/BitmapUtils.h"
#include "UIKit/PlatformUtils.h"
#include "UIKit/ResourceUtils.h"

namespace d14engine::uikit
{
    ShadowStyle::ShadowStyle(UINT bitmapWidth, UINT bitmapHeight)
    {
        loadBitmap(bitmapWidth, bitmapHeight);
    }

    void ShadowStyle::loadBitmap(UINT width, UINT height)
    {
        THROW_IF_NULL(Application::g_app);

        resource_utils::g_shadowEffect->SetInput(0, nullptr);

        auto rndr = Application::g_app->dx12Renderer();
        rndr->beginGpuCommand();

        auto dipSize = SIZE{ (LONG)width, (LONG)height };
        auto pixSize = platform_utils::scaledByDpi(dipSize);

        data = bitmap_utils::loadBitmap(
            (UINT)pixSize.cx, (UINT)pixSize.cy,
            nullptr, D2D1_BITMAP_OPTIONS_TARGET);

        rndr->endGpuCommand();
    }

    void ShadowStyle::loadBitmap(const D2D1_SIZE_U& size)
    {
        return loadBitmap(size.width, size.height);
    }

    void ShadowStyle::beginDraw(ID2D1DeviceContext* context, const D2D1_MATRIX_3X2_F& transform)
    {
        // It is recommended to call SetTarget before BeginDraw.
        // The program may crash if the previous target is a synchronized
        // resource and still bound to the context when calling BeginDraw.
        context->SetTarget(data.Get());

        context->BeginDraw();
        context->SetTransform(transform);
        context->Clear(D2D1::ColorF{ 0x000000, 0.0f });
    }

    void ShadowStyle::endDraw(ID2D1DeviceContext* context)
    {
        THROW_IF_FAILED(context->EndDraw());
        // We do not reset the target here since in beginDraw
        // SetTarget is called before BeginDraw, so the target is always valid
        // when BeginDraw called, otherwise the program may crash unexpectedly.
    }

    void ShadowStyle::configEffectInput(ID2D1Effect* effect)
    {
        effect->SetInput(0, data.Get());

        THROW_IF_FAILED(effect->SetValue(D2D1_SHADOW_PROP_COLOR, color));
        THROW_IF_FAILED(effect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, standardDeviation));
        THROW_IF_FAILED(effect->SetValue(D2D1_SHADOW_PROP_OPTIMIZATION, optimization));
    }
}
