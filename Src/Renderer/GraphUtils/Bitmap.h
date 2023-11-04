#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer::graph_utils
{
    namespace bitmap
    {
        void initialize();

        // Contrast to loading bitmap from file, storing bitmap to file
        // is a little tricky and complex. So we expose the inner factory
        // instance and allow users to implement custom storing process.

        IWICImagingFactory2* factory();

        ComPtr<IWICBitmapLock> map(
            IWICBitmapSource* source,
            DWORD flgas = WICBitmapLockRead,
            Optional<WICRect> rect = {});

        ComPtr<IWICBitmapSource> load(
            WstrParam imagePath,
            REFWICPixelFormatGUID format = GUID_WICPixelFormat32bppPRGBA);

        ComPtr<IWICBitmapSource> load(
            IStream* imageStream,
            REFWICPixelFormatGUID format = GUID_WICPixelFormat32bppPRGBA);
    }
}
