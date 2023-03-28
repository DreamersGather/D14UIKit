#include "Common/Precompile.h"

#include "Image.h"

#include "Common/DirectXError.h"
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

    bool Image::cpuRead() const
    {
        return pimpl->cpuRead;
    }

    Image::Image(Passkey)
        :
        pimpl(std::make_shared<Impl>()) { }

    void Image::initialize() { }

    Size Image::size() const
    {
        auto pxSize = pimpl->bitmap->GetPixelSize();
        return { (int)pxSize.width, (int)pxSize.height };
    }

    void Image::setSize(const Size& value)
    {
        uikit::Application::g_app->dxRenderer()->beginGpuCommand();

        auto options = !pimpl->cpuRead ? D2D1_BITMAP_OPTIONS_NONE :
            D2D1_BITMAP_OPTIONS_CANNOT_DRAW | D2D1_BITMAP_OPTIONS_CPU_READ;

        pimpl->bitmap = uikit::bitmap_utils::loadBitmap(
            value.width, value.height, nullptr, options);

        uikit::Application::g_app->dxRenderer()->endGpuCommand();
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

    void Image::load(const std::wstring& path, bool cpuRead)
    {
        pimpl->cpuRead = cpuRead;

        uikit::Application::g_app->dxRenderer()->beginGpuCommand();

        auto options = !pimpl->cpuRead ? D2D1_BITMAP_OPTIONS_NONE :
            D2D1_BITMAP_OPTIONS_CANNOT_DRAW | D2D1_BITMAP_OPTIONS_CPU_READ;

        pimpl->bitmap = uikit::bitmap_utils::loadBitmap(path, L"", options);

        uikit::Application::g_app->dxRenderer()->endGpuCommand();
    }

    bool Image::copy(Rect dst, Pixel* source)
    {
        uikit::Application::g_app->dxRenderer()->beginGpuCommand();

        D2D1_RECT_U dstRect =
        {
            (UINT)dst.left,  (UINT)dst.top,
            (UINT)dst.right, (UINT)dst.bottom
        };
        int pitch = 4 * (dst.right - dst.left);
        // The pixel format of the bitmap is hardcoded as B8G8R8A8, so the pitch
        // (byte count of each scanline) is set to "4 * dst_width" directly.
        bool status = SUCCEEDED(pimpl->bitmap->CopyFromMemory(&dstRect, source, pitch));

        uikit::Application::g_app->dxRenderer()->endGpuCommand();

        return status;
    }

    bool Image::copy(Point dst, Image* source, Rect src)
    {
        uikit::Application::g_app->dxRenderer()->beginGpuCommand();
        
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
        bool status = SUCCEEDED(pimpl->bitmap->CopyFromBitmap(
            &destPoint, source->pimpl->bitmap.Get(), &srcRect));

        uikit::Application::g_app->dxRenderer()->endGpuCommand();

        return status;
    }

    Pixel* Image::map()
    {
        if (!pimpl->cpuRead) return nullptr;

        uikit::Application::g_app->dxRenderer()->beginGpuCommand();

        constexpr auto options = D2D1_MAP_OPTIONS_READ;
        D2D1_MAPPED_RECT mapped = {};

        bool status = SUCCEEDED(pimpl->bitmap->Map(options, &mapped));

        uikit::Application::g_app->dxRenderer()->endGpuCommand();

        return status ? (Pixel*)mapped.bits : nullptr;
    }

    bool Image::unmap() { return SUCCEEDED(pimpl->bitmap->Unmap()); }
}
