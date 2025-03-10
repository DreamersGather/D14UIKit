﻿#include "Common/Precompile.h"

#include "Image.h"

#include "Common/DirectXError.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/Application.h"
#include "UIKit/BitmapUtils.h"

using namespace d14engine;

namespace d14uikit
{
    Image::Image(int width, int height, bool cpuRead)
        :
        Image(Passkey{})
    {
        pimpl->cpuRead = cpuRead;
        setSize({ width, height });
    }

    Image::Image(const std::wstring& path, bool cpuRead)
        :
        Image(Passkey{})
    {
        load(path, cpuRead);
    }

    Image::Image(Passkey)
        :
        pimpl(std::make_shared<Impl>()) { }

    bool Image::cpuRead() const
    {
        return pimpl->cpuRead;
    }

    Size Image::size() const
    {
        auto pxSize = pimpl->bitmap->GetPixelSize();
        return { (int)pxSize.width, (int)pxSize.height };
    }

    void Image::setSize(const Size& value)
    {
        uikit::Application::g_app->dx12Renderer()->beginGpuCommand();

        auto options = !pimpl->cpuRead ? D2D1_BITMAP_OPTIONS_NONE :
            D2D1_BITMAP_OPTIONS_CANNOT_DRAW | D2D1_BITMAP_OPTIONS_CPU_READ;

        pimpl->bitmap = uikit::bitmap_utils::loadBitmap(
            value.width, value.height, nullptr, options);

        uikit::Application::g_app->dx12Renderer()->endGpuCommand();
    }

    int Image::width() const
    {
        return size().width;
    }

    void Image::setWidth(int value)
    {
        setSize({ value, height() });
    }

    int Image::height() const
    {
        return size().height;
    }

    void Image::setHeight(int value)
    {
        setSize({ width(), value });
    }

    Size Image::dipSize() const
    {
        auto size = pimpl->bitmap->GetSize();
        return
        {
            math_utils::round(size.width),
            math_utils::round(size.height)
        };
    }

    int Image::dipWidth() const
    {
        return dipSize().width;
    }

    int Image::dipHeight() const
    {
        return dipSize().height;
    }

    void Image::load(const std::wstring& path, bool cpuRead)
    {
        pimpl->cpuRead = cpuRead;

        uikit::Application::g_app->dx12Renderer()->beginGpuCommand();

        auto options = !pimpl->cpuRead ? D2D1_BITMAP_OPTIONS_NONE :
            D2D1_BITMAP_OPTIONS_CANNOT_DRAW | D2D1_BITMAP_OPTIONS_CPU_READ;

        pimpl->bitmap = uikit::bitmap_utils::loadBitmap(path, options);

        uikit::Application::g_app->dx12Renderer()->endGpuCommand();
    }

    void Image::save(const std::wstring& path, Format format)
    {
        uikit::Application::g_app->dx12Renderer()->waitGpuCommand();

        GUID formatID = {};
        switch (format)
        {
        case Adng: formatID = GUID_ContainerFormatAdng; break;
        case Bmp: formatID = GUID_ContainerFormatBmp; break;
        case Png: formatID = GUID_ContainerFormatPng; break;
        case Ico: formatID = GUID_ContainerFormatIco; break;
        case Jpeg: formatID = GUID_ContainerFormatJpeg; break;
        case Tiff: formatID = GUID_ContainerFormatTiff; break;
        case Gif: formatID = GUID_ContainerFormatGif; break;
        case Wmp: formatID = GUID_ContainerFormatWmp; break;
        case Heif: formatID = GUID_ContainerFormatHeif; break;
        case Webp: formatID = GUID_ContainerFormatWebp; break;
        default: formatID = GUID_ContainerFormatPng; break;
        }
        uikit::bitmap_utils::saveBitmap(pimpl->bitmap.Get(), path, formatID);
    }

    void Image::copy(const Rect& dst, const Pixel* source)
    {
        uikit::Application::g_app->dx12Renderer()->beginGpuCommand();

        copyInFrame(dst, source);

        uikit::Application::g_app->dx12Renderer()->endGpuCommand();
    }

    void Image::copy(const Point& dst, Image* source, const Rect& src)
    {
        uikit::Application::g_app->dx12Renderer()->beginGpuCommand();

        copyInFrame(dst, source, src);

        uikit::Application::g_app->dx12Renderer()->endGpuCommand();
    }

    void Image::copyInFrame(const Rect& dst, const Pixel* source)
    {
        D2D1_RECT_U dstRect =
        {
            (UINT)dst.left,  (UINT)dst.top,
            (UINT)dst.right, (UINT)dst.bottom
        };
        UINT32 pitch = 4 * (dstRect.right - dstRect.left);
        // The pixel format of the bitmap is hardcoded as R8G8B8A8, so the pitch
        // (byte count of each scanline) is set to "4 * dst_width" directly.
        THROW_IF_FAILED(pimpl->bitmap->CopyFromMemory(&dstRect, source, pitch));
    }

    void Image::copyInFrame(const Point& dst, Image* source, const Rect& src)
    {
        D2D1_RECT_U srcRect =
        {
            (UINT)src.left,  (UINT)src.top,
            (UINT)src.right, (UINT)src.bottom
        };
        D2D1_POINT_2U destPoint =
        {
            (UINT)dst.x, (UINT)dst.y
        };
        // This doesn't work if either the source or destination bitmap is mapped.
        THROW_IF_FAILED(pimpl->bitmap->CopyFromBitmap(
            &destPoint, source->pimpl->bitmap.Get(), &srcRect));
    }

    Pixel* Image::map()
    {
        if (!pimpl->cpuRead) return nullptr;

        uikit::Application::g_app->dx12Renderer()->beginGpuCommand();

        constexpr auto options = D2D1_MAP_OPTIONS_READ;
        D2D1_MAPPED_RECT mapped = {};

        THROW_IF_FAILED(pimpl->bitmap->Map(options, &mapped));

        uikit::Application::g_app->dx12Renderer()->endGpuCommand();

        return (Pixel*)mapped.bits;
    }

    void Image::unmap() { THROW_IF_FAILED(pimpl->bitmap->Unmap()); }
}
