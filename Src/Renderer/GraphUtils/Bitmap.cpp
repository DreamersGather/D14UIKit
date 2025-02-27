#include "Common/Precompile.h"

#include "Renderer/GraphUtils/Bitmap.h"

#include "Common/DirectXError.h"

namespace d14engine::renderer::graph_utils
{
    namespace bitmap
    {
        ComPtr<IWICImagingFactory2> g_factory = {};

        void initialize()
        {
            THROW_IF_FAILED(CoInitializeEx
            (
            /* pvReserved */ nullptr,
            /* dwCoInit   */ COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
            ));
            THROW_IF_FAILED(CoCreateInstance
            (
            /* rclsid       */ CLSID_WICImagingFactory,
            /* pUnkOuter    */ nullptr,
            /* dwClsContext */ CLSCTX_INPROC_SERVER,
            /* riid         */
            /* ppv          */ IID_PPV_ARGS(&g_factory)
            ));
        }

        IWICImagingFactory2* factory()
        {
            return g_factory.Get();
        }

        ComPtr<IWICBitmapLock> map(
            IWICBitmapSource* source,
            DWORD flgas,
            Optional<WICRect> rect)
        {
            ComPtr<IWICBitmap> bitmap = {};
            THROW_IF_FAILED(factory()->CreateBitmapFromSource
            (
            /* pIBitmapSource */ source,
            /* option         */ WICBitmapNoCache,
            /* ppIBitmap      */ &bitmap)
            );
            WICRect rc = {};
            if (rect.has_value())
            {
                rc = rect.value();
            }
            else // user-defined
            {
                rc.X = rc.Y = 0;
                THROW_IF_FAILED(bitmap->GetSize((UINT*)&rc.Width, (UINT*)&rc.Height));
            }
            ComPtr<IWICBitmapLock> lock = {};
            THROW_IF_FAILED(bitmap->Lock(&rc, flgas, &lock));

            return lock;
        }

        ComPtr<IWICBitmapSource> load(
            WstrParam imagePath,
            REFWICPixelFormatGUID format)
        {
            ComPtr<IWICBitmapDecoder> decoder = {};
            THROW_IF_FAILED(g_factory->CreateDecoderFromFilename
            (
            /* wzFilename      */ imagePath.c_str(),
            /* pguidVendor     */ nullptr,
            /* dwDesiredAccess */ GENERIC_READ,
            /* metadataOptions */ WICDecodeMetadataCacheOnDemand,
            /* ppIDecoder      */ &decoder
            ));
            ComPtr<IWICBitmapFrameDecode> frameDecode = {};
            THROW_IF_FAILED(decoder->GetFrame(0, &frameDecode));

            ComPtr<IWICFormatConverter> formatConverter = {};
            THROW_IF_FAILED(g_factory->CreateFormatConverter(&formatConverter));

            THROW_IF_FAILED(formatConverter->Initialize
            (
            /* pISource              */ frameDecode.Get(),
            /* dstFormat             */ format,
            /* dither                */ WICBitmapDitherTypeNone,
            /* pIPalette             */ nullptr,
            /* alphaThresholdPercent */ 0.0f,
            /* paletteTranslate      */ WICBitmapPaletteTypeCustom
            ));
            return formatConverter;
        }

        ComPtr<IWICBitmapSource> load(
            IStream* imageStream,
            REFWICPixelFormatGUID format)
        {
            ComPtr<IWICBitmapDecoder> decoder = {};
            THROW_IF_FAILED(g_factory->CreateDecoderFromStream
            (
            /* pIStream        */ imageStream,
            /* pguidVendor     */ nullptr,
            /* metadataOptions */ WICDecodeMetadataCacheOnDemand,
            /* ppIDecoder      */ &decoder
            ));
            ComPtr<IWICBitmapFrameDecode> frameDecode = {};
            THROW_IF_FAILED(decoder->GetFrame(0, &frameDecode));

            ComPtr<IWICFormatConverter> formatConverter = {};
            THROW_IF_FAILED(g_factory->CreateFormatConverter(&formatConverter));

            THROW_IF_FAILED(formatConverter->Initialize
            (
            /* pISource              */ frameDecode.Get(),
            /* dstFormat             */ format,
            /* dither                */ WICBitmapDitherTypeNone,
            /* pIPalette             */ nullptr,
            /* alphaThresholdPercent */ 0.0f,
            /* paletteTranslate      */ WICBitmapPaletteTypeCustom
            ));
            return formatConverter;
        }
    }
}
