#include "Common/Precompile.h"

#include "UIKit/MaskStyle.h"

#include "Common/DirectXError.h"

#include "UIKit/Application.h"
#include "UIKit/BitmapUtils.h"

namespace d14engine::uikit
{
    MaskStyle::MaskStyle(UINT bitmapWidth, UINT bitmapHeight)
    {
        loadMaskBitmap(bitmapWidth, bitmapHeight);
    }

    void MaskStyle::loadMaskBitmap(UINT width, UINT height)
    {
        auto rndr = Application::g_app->dxRenderer();
        rndr->beginGpuCommand();

        bitmap = bitmap_utils::loadBitmap(
            width, height, nullptr, D2D1_BITMAP_OPTIONS_TARGET);

        rndr->endGpuCommand();
    }

    void MaskStyle::loadMaskBitmap(const D2D1_SIZE_U& size)
    {
        loadMaskBitmap(size.width, size.height);
    }

    void MaskStyle::beginMaskDraw(ID2D1DeviceContext* context, const D2D1_MATRIX_3X2_F& transform)
    {
        context->SetTarget(bitmap.Get());
        context->BeginDraw();
        context->SetTransform(transform);
        context->Clear(color);
    }

    void MaskStyle::endMaskDraw(ID2D1DeviceContext* context)
    {
        THROW_IF_FAILED(context->EndDraw());
        // We do not reset the target here since in beginMaskDraw
        // SetTarget is called before BeginDraw, so the target is always valid
        // when BeginDraw called, otherwise the program may crash unexpectedly.
    }
}
