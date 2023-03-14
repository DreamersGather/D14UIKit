#include "Common/Precompile.h"

#include "Image.h"

#include "Common/DirectXError.h"
#include "UIKit/Application.h"
#include "UIKit/BitmapUtils.h"

using namespace d14engine;

namespace d14uikit
{
    Image::Image() : Image(Passkey{}) { }

    Image::Image(int width, int height)
        :
        Image(Passkey{})
    {
        setSize({ width, height });
    }

    Image::Image(const std::wstring& path)
        :
        Image(Passkey{}) { load(path); }

    Image::Image(Passkey)
        :
        pimpl(std::make_shared<Impl>()) { }

    void Image::initialize() { }

    Size Image::size() const
    {
        auto pxSize = pimpl->bitmap->GetPixelSize();
        return { (int)pxSize.width, (int)pxSize.height };
    }

    void Image::setSize(Size value)
    {
        uikit::Application::g_app->dxRenderer()->beginGpuCommand();

        pimpl->bitmap = uikit::bitmap_utils::loadBitmap(value.width, value.height);

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

    void Image::load(const std::wstring& path)
    {
        uikit::Application::g_app->dxRenderer()->beginGpuCommand();

        pimpl->bitmap = uikit::bitmap_utils::loadBitmap(path);

        uikit::Application::g_app->dxRenderer()->endGpuCommand();
    }

    void Image::copy(Point offset, Image* source, Rect srcArea)
    {
        uikit::Application::g_app->dxRenderer()->beginGpuCommand();

        D2D1_POINT_2U dstpt =
        {
            (UINT)offset.x, (UINT)offset.y
        };
        D2D1_RECT_U srcrect =
        {
            (UINT)srcArea.left, (UINT)srcArea.top,
            (UINT)srcArea.right, (UINT)srcArea.bottom
        };
        THROW_IF_FAILED(pimpl->bitmap->CopyFromBitmap(&dstpt, source->pimpl->bitmap.Get(), &srcrect));

        uikit::Application::g_app->dxRenderer()->endGpuCommand();
    }
}
