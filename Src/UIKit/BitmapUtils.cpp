#include "Common/Precompile.h"

#include "UIKit/BitmapUtils.h"

#include "Common/DirectXError.h"
#include "Common/ResourcePack.h"

#include "Renderer/Renderer.h"

#include "UIKit/Application.h"
#include "UIKit/PlatformUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit::bitmap_utils
{
    ComPtr<IWICImagingFactory2> g_imageFactory = {};

    void initialize()
    {
        THROW_IF_FAILED(CoInitializeEx(
            nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));

        THROW_IF_FAILED(CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&g_imageFactory)));
    }

    ComPtr<ID2D1Bitmap1> loadBitmap(UINT width, UINT height, BYTE* data, D2D1_BITMAP_OPTIONS options)
    {
        auto dpi = platform_utils::dpi();

        D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1
        (
            /* bitmapOptions */ options,
            /* pixelFormat   */ D2D1::PixelFormat(Renderer::g_renderTargetFormat, D2D1_ALPHA_MODE_PREMULTIPLIED),
            /* dpiX          */ dpi,
            /* dpiY          */ dpi
        );
        ComPtr<ID2D1Bitmap1> bitmap;
        // Hardcode the pixel format as R8G8B8A8 for simplicity, so the pitch
        // (byte count of each scanline) is set to "4 * width" directly.
        THROW_IF_FAILED(Application::g_app->dxRenderer()->d2d1DeviceContext()->CreateBitmap
        (
            /* size             */ { width, height },
            /* sourceData       */ data,
            /* pitch            */ 4 * width,
            /* bitmapProperties */ props,
            /* bitmap           */ &bitmap
        ));
        return bitmap;
    }

    ComPtr<ID2D1Bitmap1> loadBitmap(WstrParam imageFile, WstrParam binaryPath, D2D1_BITMAP_OPTIONS options)
    {
        ComPtr<IWICBitmapDecoder> decoder;
        THROW_IF_FAILED(g_imageFactory->CreateDecoderFromFilename(
            (binaryPath + imageFile).c_str(),
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnDemand,
            &decoder));

        ComPtr<IWICBitmapFrameDecode> frameDecode;
        THROW_IF_FAILED(decoder->GetFrame(0, &frameDecode));

        ComPtr<IWICFormatConverter> formatConverter;
        THROW_IF_FAILED(g_imageFactory->CreateFormatConverter(&formatConverter));

        THROW_IF_FAILED(formatConverter->Initialize(
            frameDecode.Get(),
            GUID_WICPixelFormat32bppPRGBA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0f,
            WICBitmapPaletteTypeCustom));

        D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1
        (
            /* bitmapOptions */ options,
            /* pixelFormat   */ D2D1::PixelFormat(Renderer::g_renderTargetFormat, D2D1_ALPHA_MODE_PREMULTIPLIED),
            /* dpiX          */ 0.0f,
            /* dpiY          */ 0.0f
        );
        ComPtr<ID2D1Bitmap1> bitmap;
        THROW_IF_FAILED(Application::g_app->dxRenderer()->d2d1DeviceContext()->CreateBitmapFromWicBitmap
        (
            /* wicBitmapSource  */ formatConverter.Get(),
            /* bitmapProperties */ props,
            /* bitmap           */ &bitmap
        ));
        return bitmap;
    }

    ComPtr<ID2D1Bitmap1> loadPackedBitmap(WstrParam resName, WstrParam resType, D2D1_BITMAP_OPTIONS options)
    {
        // Create stream from memory.

        auto src = loadResource(resName, resType);

        HGLOBAL	mem = GlobalAlloc(GMEM_MOVEABLE, src.size);
        THROW_IF_NULL(mem);

        LPVOID dst = GlobalLock(mem);
        THROW_IF_NULL(dst);
        memcpy(dst, src.data, src.size);
        GlobalUnlock(mem);

        ComPtr<IStream> stream;
        THROW_IF_FAILED(CreateStreamOnHGlobal(mem, FALSE, &stream));

        // Create bitmap from stream.

        ComPtr<IWICBitmapDecoder> decoder;
        THROW_IF_FAILED(g_imageFactory->CreateDecoderFromStream(
            stream.Get(),
            nullptr,
            WICDecodeMetadataCacheOnDemand,
            &decoder));

        ComPtr<IWICBitmapFrameDecode> frameDecode;
        THROW_IF_FAILED(decoder->GetFrame(0, &frameDecode));

        ComPtr<IWICFormatConverter> formatConverter;
        THROW_IF_FAILED(g_imageFactory->CreateFormatConverter(&formatConverter));

        THROW_IF_FAILED(formatConverter->Initialize(
            frameDecode.Get(),
            GUID_WICPixelFormat32bppPRGBA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0f,
            WICBitmapPaletteTypeCustom));

        D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1
        (
            /* bitmapOptions */ options,
            /* pixelFormat   */ D2D1::PixelFormat(Renderer::g_renderTargetFormat, D2D1_ALPHA_MODE_PREMULTIPLIED),
            /* dpiX          */ 0.0f,
            /* dpiY          */ 0.0f
        );
        ComPtr<ID2D1Bitmap1> bitmap;
        THROW_IF_FAILED(Application::g_app->dxRenderer()->d2d1DeviceContext()->CreateBitmapFromWicBitmap
        (
            /* wicBitmapSource  */ formatConverter.Get(),
            /* bitmapProperties */ props,
            /* bitmap           */ &bitmap
        ));
        return bitmap;
    }

    void saveBitmap(ID2D1Bitmap1* bitmap, WstrParam imageFile, const GUID& format)
    {
        // Create file and stream.

        ComPtr<IWICStream> stream;
        THROW_IF_FAILED(g_imageFactory->CreateStream(&stream));

        THROW_IF_FAILED(stream->InitializeFromFilename(imageFile.c_str(), GENERIC_WRITE));

        // Export image to stream.

        ComPtr<IWICBitmapEncoder> bitmapEncoder;
        THROW_IF_FAILED(g_imageFactory->CreateEncoder(format, nullptr, &bitmapEncoder));

        THROW_IF_FAILED(bitmapEncoder->Initialize(stream.Get(), WICBitmapEncoderNoCache));

        ComPtr<IWICBitmapFrameEncode> frameEncode;
        THROW_IF_FAILED(bitmapEncoder->CreateNewFrame(&frameEncode, nullptr));

        THROW_IF_FAILED(frameEncode->Initialize(nullptr));

        auto device = Application::g_app->dxRenderer()->d2d1Device();

        ComPtr<IWICImageEncoder> imageEncoder;
        THROW_IF_FAILED(g_imageFactory->CreateImageEncoder(device, &imageEncoder));

        WICImageParameters imageParams = {};
        imageParams.PixelFormat = bitmap->GetPixelFormat();
        bitmap->GetDpi(&imageParams.DpiX, &imageParams.DpiY);
        imageParams.Top = 0.0f;
        imageParams.Left = 0.0f;
        auto pixSize = bitmap->GetPixelSize();
        imageParams.PixelWidth = pixSize.width;
        imageParams.PixelHeight = pixSize.height;

        THROW_IF_FAILED(imageEncoder->WriteFrame(bitmap, frameEncode.Get(), &imageParams));

        THROW_IF_FAILED(frameEncode->Commit());
        THROW_IF_FAILED(bitmapEncoder->Commit());

        THROW_IF_FAILED(stream->Commit(STGC_DEFAULT));
    }
}
