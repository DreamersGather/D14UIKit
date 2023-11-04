#include "Common/Precompile.h"

#include "Renderer/GraphUtils/Bitmap.h"

#include "Common/DirectXError.h"

namespace d14engine::renderer::graph_utils
{
    namespace bitmap
    {
        ComPtr<IWICImagingFactory2> g_imagingFactory = {};

        void initialize()
        {
            THROW_IF_FAILED(CoInitializeEx(
                nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));

            THROW_IF_FAILED(CoCreateInstance(
                CLSID_WICImagingFactory,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&g_imagingFactory)));
        }

        IWICImagingFactory2* factory()
        {
            return g_imagingFactory.Get();
        }

        ComPtr<IWICBitmapLock> map(IWICBitmapSource* source, DWORD flgas, Optional<WICRect> rect)
        {
            ComPtr<IWICBitmap> bitmap;
            THROW_IF_FAILED(graph_utils::bitmap::factory()->CreateBitmapFromSource(
                source,
                WICBitmapNoCache,
                &bitmap));

            if (!rect.has_value())
            {
                auto& value = (rect = { .X = 0, .Y = 0 }).value();
                THROW_IF_FAILED(bitmap->GetSize((UINT*)&value.Width, (UINT*)&value.Height));
            }
            ComPtr<IWICBitmapLock> lock;
            THROW_IF_FAILED(bitmap->Lock(&rect.value(), flgas, &lock));

            return lock;
        }

        ComPtr<IWICBitmapSource> load(WstrParam imagePath, REFWICPixelFormatGUID format)
        {
            ComPtr<IWICBitmapDecoder> decoder;
            THROW_IF_FAILED(g_imagingFactory->CreateDecoderFromFilename(
                imagePath.c_str(),
                nullptr,
                GENERIC_READ,
                WICDecodeMetadataCacheOnDemand,
                &decoder));

            ComPtr<IWICBitmapFrameDecode> frameDecode;
            THROW_IF_FAILED(decoder->GetFrame(0, &frameDecode));

            ComPtr<IWICFormatConverter> formatConverter;
            THROW_IF_FAILED(g_imagingFactory->CreateFormatConverter(&formatConverter));

            THROW_IF_FAILED(formatConverter->Initialize(
                frameDecode.Get(),
                format,
                WICBitmapDitherTypeNone,
                nullptr,
                0.0f,
                WICBitmapPaletteTypeCustom));

            return formatConverter;
        }

        ComPtr<IWICBitmapSource> load(IStream* imageStream, REFWICPixelFormatGUID format)
        {
            ComPtr<IWICBitmapDecoder> decoder;
            THROW_IF_FAILED(g_imagingFactory->CreateDecoderFromStream(
                imageStream,
                nullptr,
                WICDecodeMetadataCacheOnDemand,
                &decoder));

            ComPtr<IWICBitmapFrameDecode> frameDecode;
            THROW_IF_FAILED(decoder->GetFrame(0, &frameDecode));

            ComPtr<IWICFormatConverter> formatConverter;
            THROW_IF_FAILED(g_imagingFactory->CreateFormatConverter(&formatConverter));

            THROW_IF_FAILED(formatConverter->Initialize(
                frameDecode.Get(),
                format,
                WICBitmapDitherTypeNone,
                nullptr,
                0.0f,
                WICBitmapPaletteTypeCustom));

            return formatConverter;
        }
    }
}
