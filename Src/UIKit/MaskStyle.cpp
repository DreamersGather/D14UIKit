#include "Common/Precompile.h"

#include "UIKit/MaskStyle.h"

#include "Common/DirectXError.h"

#include "UIKit/Application.h"
#include "UIKit/BitmapUtils.h"
#include "UIKit/PlatformUtils.h"

namespace d14engine::uikit
{
    MaskStyle::MaskStyle(UINT bitmapWidth, UINT bitmapHeight)
    {
        loadBitmap(bitmapWidth, bitmapHeight);
    }

    void MaskStyle::loadBitmap(UINT width, UINT height)
    {
        THROW_IF_NULL(Application::g_app);

        auto rndr = Application::g_app->dx12Renderer();
        rndr->beginGpuCommand();

        auto dipSize = SIZE{ (LONG)width, (LONG)height };
        auto pixSize = platform_utils::scaledByDpi(dipSize);

        data = bitmap_utils::loadBitmap(
            (UINT)pixSize.cx, (UINT)pixSize.cy,
            nullptr, D2D1_BITMAP_OPTIONS_TARGET);

        rndr->endGpuCommand();
    }

    void MaskStyle::loadBitmap(const D2D1_SIZE_U& size)
    {
        loadBitmap(size.width, size.height);
    }

    void MaskStyle::beginDraw(ID2D1DeviceContext* context, const D2D1_MATRIX_3X2_F& transform)
    {
        // It is recommended to call SetTarget before BeginDraw.
        // The program may crash if the previous target is a synchronized
        // resource and still bound to the context when calling BeginDraw.
        context->SetTarget(data.Get());
        context->BeginDraw();
        context->SetTransform(transform);
        context->Clear(color);
    }

    void MaskStyle::endDraw(ID2D1DeviceContext* context)
    {
        THROW_IF_FAILED(context->EndDraw());
        // We do not reset the target here since in beginDraw
        // SetTarget is called before BeginDraw, so the target is always valid
        // when BeginDraw called, otherwise the program may crash unexpectedly.
    }
}
