#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer::graph_utils
{
    namespace bitmap
    {
        void initialize();

        // Contrast to loading bitmap from file, saving bitmap to file
        // depends on more specific settings. So we expose the factory
        // instance and allow users to implement custom saving process.

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
