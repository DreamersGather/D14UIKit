#include "Common/Precompile.h"

#include "Application.h"

#include "Common.h"
#include "Cursor.h"
#include "Image.h"
#include "Renderer.h"

#include "Common/MathUtils/Basic.h"

#include "Renderer/TickTimer.h"

#include "UIKit/Application.h"
#include "UIKit/BitmapObject.h"
#include "UIKit/ColorUtils.h"
#include "UIKit/Cursor.h"
#include "UIKit/PlatformUtils.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    Application* Application::Impl::app = nullptr;

    using CreateInfo = uikit::Application::CreateInfo;

    Application::Application(
        const std::wstring& name,
        const std::optional<float>& dpi)
        :
        Application(std::make_shared<uikit::Application>(
            0, nullptr, CreateInfo{ .name = name, .dpi = dpi })) { }

    _D14_UIKIT_CTOR(Application)
        :
        pimpl(std::make_shared<Impl>())
    {
        Impl::app = this;

        pimpl->uiobj = uiobj;

        // Binds renderer::Renderer to d14uikit::Renderer
        {
            auto rndr = pimpl->uiobj->dx12Renderer();
            auto rndrCast = std::make_shared<uikit::Renderer>(rndr);
            pimpl->rndr = std::shared_ptr<Renderer>(new Renderer(rndrCast));
        }
        pimpl->uiobj->f_onSystemThemeStyleChange = [this]
        {
            if (pimpl->useSystemTheme)
            {
                auto& mode = pimpl->uiobj->systemThemeStyle().mode;
                using ThemeMode = uikit::Application::ThemeStyle::Mode;

                if (mode == ThemeMode::Light)
                {
                    pimpl->uiobj->changeTheme(L"Light");
                }
                else if (mode == ThemeMode::Dark)
                {
                    pimpl->uiobj->changeTheme(L"Dark");
                }
            }
        };
        _D14_UIKIT_BIND(Cursor, cursor);
    }

    Application::~Application()
    {
        Impl::app = nullptr;

        // fix memory leak reported by d2d debug layer
        uikit::resource_utils::g_solidColorBrush.Reset();
        uikit::resource_utils::g_shadowEffect.Reset();
    }

    Application* Application::app()
    {
        return Impl::app;
    }

    int Application::dpi() const
    {
        return math_utils::round(uikit::platform_utils::dpi());
    }

    BitmapInterpMode Application::bitmapInterpMode() const
    {
        return pimpl->bitmapInterpMode;
    }

    void Application::setBitmapInterpMode(BitmapInterpMode mode)
    {
        uikit::BitmapObject::g_interpolationMode = (D2D1_INTERPOLATION_MODE)mode;
    }

    int Application::run() const
    {
        return pimpl->uiobj->run();
    }

    void Application::exit() const
    {
        pimpl->uiobj->exit();
    }

    HWND Application::win32Window() const
    {
        return pimpl->uiobj->win32Window();
    }

    bool Application::visible() const
    {
        return IsWindowVisible(pimpl->uiobj->win32Window());
    }

    void Application::setVisible(bool value)
    {
        auto showFlag = value ? SW_SHOW : SW_HIDE;
        ShowWindow(pimpl->uiobj->win32Window(), showFlag);
    }

    bool Application::minimized() const
    {
        return IsMinimized(pimpl->uiobj->win32Window());
    }

    void Application::setMinimized(bool value)
    {
        if (!value && !minimized()) return;
        auto showFlag = value ? SW_MINIMIZE : SW_NORMAL;
        ShowWindow(pimpl->uiobj->win32Window(), showFlag);
    }

    bool Application::maximized() const
    {
        return IsMaximized(pimpl->uiobj->win32Window());
    }

    void Application::setMaximized(bool value)
    {
        if (!value && !maximized()) return;
        auto showFlag = value ? SW_MAXIMIZE : SW_NORMAL;
        ShowWindow(pimpl->uiobj->win32Window(), showFlag);
    }

    static Size scaledByDpi(const Size& sz)
    {
        auto factor = uikit::platform_utils::dpi() / 96.0f;
        return
        {
            math_utils::round((float)sz.width * factor),
            math_utils::round((float)sz.height * factor)
        };
    }

    static Size restoredByDpi(const Size& sz)
    {
        auto factor = 96.0f / uikit::platform_utils::dpi();
        return
        {
            math_utils::round((float)sz.width * factor),
            math_utils::round((float)sz.height * factor)
        };
    }

    static Point scaledByDpi(const Point& pt)
    {
        auto factor = uikit::platform_utils::dpi() / 96.0f;
        return
        {
            math_utils::round((float)pt.x * factor),
            math_utils::round((float)pt.y * factor)
        };
    }

    static Point restoredByDpi(const Point& pt)
    {
        auto factor = 96.0f / uikit::platform_utils::dpi();
        return
        {
            math_utils::round((float)pt.x * factor),
            math_utils::round((float)pt.y * factor)
        };
    }

    Size Application::size() const
    {
        RECT rc = {};
        GetClientRect(pimpl->uiobj->win32Window(), &rc);
        return restoredByDpi(Size
        {
            rc.right - rc.left,
            rc.bottom - rc.top
        });
    }

    void Application::setSize(const Size& value)
    {
        Size sz = scaledByDpi(value);
        SetWindowPos(
            pimpl->uiobj->win32Window(),
            HWND_TOP,
            0, // nomove
            0, // nomove
            sz.width,
            sz.height,
            SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE);
    }

    int Application::width() const
    {
        return size().width;
    }

    void Application::setWidth(int value)
    {
        setSize({ value, height() });
    }

    int Application::height() const
    {
        return size().height;
    }

    void Application::setHeight(int value)
    {
        setSize({ width(), value });
    }

    Point Application::position() const
    {
        RECT rc = {};
        GetClientRect(pimpl->uiobj->win32Window(), &rc);
        return restoredByDpi(Point{ rc.left, rc.top });
    }

    void Application::setPosition(const Point& value)
    {
        auto pt = scaledByDpi(value);
        SetWindowPos(
            pimpl->uiobj->win32Window(),
            HWND_TOP,
            pt.x,
            pt.y,
            0, // nosize
            0, // nosize
            SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOSIZE);
    }

    int Application::x() const
    {
        return position().x;
    }

    void Application::setX(int value)
    {
        setPosition({ value, y() });
    }

    int Application::y() const
    {
        return position().y;
    }

    void Application::setY(int value)
    {
        setPosition({ x(), value });
    }

    Size Application::minSize() const
    {
        auto& geo = pimpl->uiobj->win32WindowSettings.geometry;
        if (geo.minTrackSize.has_value())
        {
            auto& minsz = geo.minTrackSize.value();
            return { minsz.x, minsz.y };
        }
        else return { 0, 0 };
    }

    void Application::setMinSize(const Size& value)
    {
        auto& geo = pimpl->uiobj->win32WindowSettings.geometry;
        geo.minTrackSize = { value.width, value.height };
    }

    int Application::minWidth() const
    {
        return minSize().width;
    }

    void Application::setMinWidth(int value)
    {
        setMinSize({ value, minHeight() });
    }

    int Application::minHeight() const
    {
        return minSize().height;
    }

    void Application::setMinHeight(int value)
    {
        setMinSize({ minWidth(), value });
    }

    bool Application::resizable() const
    {
        auto& szframe = pimpl->uiobj->win32WindowSettings.sizingFrame;
        return szframe.frameWidth.has_value();
    }

    void Application::setResizable(bool value)
    {
        auto& szframe = pimpl->uiobj->win32WindowSettings.sizingFrame;
        if (value)
        {
            szframe.frameWidth = 6;
        }
        else szframe.frameWidth.reset();
    }

    Renderer* Application::dx12Renderer() const
    {
        return pimpl->rndr.get();
    }

    bool Application::fullscreen() const
    {
        return pimpl->uiobj->dx12Renderer()->window().fullscreen();
    }

    void Application::setFullscreen(bool value)
    {
        auto& dxwnd = pimpl->uiobj->dx12Renderer()->window();
        if (value) dxwnd.enterFullscreenMode();
        else dxwnd.restoreWindowedMode();
    }

    int Application::fps() const
    {
        return pimpl->uiobj->dx12Renderer()->timer()->fps();
    }

    std::unique_ptr<Image> Application::capture() const
    {
        std::unique_ptr<Image> screenshot(new Image(Image::Passkey{}));
        screenshot->getImpl()->bitmap = pimpl->uiobj->screenshot();
        return screenshot;
    }

    TextAntialiasMode Application::textAntialiasMode() const
    {
        return pimpl->textAntialiasMode;
    }

    void Application::setTextAntialiasMode(TextAntialiasMode mode)
    {
        if (pimpl->textAntialiasMode != mode)
        {
            auto rndr = pimpl->uiobj->dx12Renderer();
            rndr->setTextAntialiasMode((D2D1_TEXT_ANTIALIAS_MODE)mode);
        }
        pimpl->textAntialiasMode = mode;
    }

    RenderingMode Application::renderingMode() const
    {
        return pimpl->renderingMode;
    }

    void Application::setRenderingMode(RenderingMode mode)
    {
        if (pimpl->renderingMode != mode)
        {
            auto defMode = pimpl->uiobj->dx12Renderer()->getDefaultTextRenderingMode();
            defMode.renderingMode = (DWRITE_RENDERING_MODE)mode;
            pimpl->uiobj->dx12Renderer()->setTextRenderingMode(defMode);
        }
        pimpl->renderingMode = mode;
    }

    int Application::animCount() const
    {
        return pimpl->uiobj->animationCount();
    }

    bool Application::animState() const
    {
        return pimpl->animState;
    }

    void Application::setAnimState(bool value)
    {
        if (pimpl->animState != value)
        {
            if (value)
            {
                pimpl->uiobj->increaseAnimationCount();
            }
            else pimpl->uiobj->decreaseAnimationCount();
        }
        pimpl->animState = value;
    }

    Cursor* Application::cursor() const
    {
        return pimpl->cursor.get();
    }

    const std::wstring& Application::themeMode() const
    {
        return pimpl->uiobj->currThemeName();
    }

    void Application::setThemeMode(const std::wstring& name)
    {
        if (name != L"Light" && name != L"Dark") return;

        if (useSystemTheme()) setUseSystemTheme(false);

        auto& style = pimpl->uiobj->customThemeStyle.value();
        using ThemeMode = uikit::Application::ThemeStyle::Mode;

        if (name == L"Light") style.mode = ThemeMode::Light;
        else if (name == L"Dark") style.mode = ThemeMode::Dark;

        pimpl->uiobj->changeTheme(name);
    }

    Color Application::themeColor() const
    {
        uikit::Application::ThemeStyle style = {};
        if (pimpl->uiobj->customThemeStyle.has_value())
        {
            style = pimpl->uiobj->customThemeStyle.value();
        }
        else style = pimpl->uiobj->systemThemeStyle();

        return convert(style.color);
    }

    void Application::setThemeColor(const Color& value)
    {
        if (useSystemTheme()) setUseSystemTheme(false);

        pimpl->uiobj->customThemeStyle.value().color = convert(value);

        pimpl->uiobj->changeTheme(pimpl->uiobj->currThemeName());
    }

    bool Application::useSystemTheme() const
    {
        return pimpl->useSystemTheme;
    }

    void Application::setUseSystemTheme(bool value)
    {
        if (pimpl->useSystemTheme && !value)
        {
            auto& systemStyle = pimpl->uiobj->systemThemeStyle();
            pimpl->uiobj->customThemeStyle = systemStyle;
        }
        else pimpl->uiobj->customThemeStyle.reset();

        pimpl->useSystemTheme = value;
        if (pimpl->useSystemTheme)
        {
            pimpl->uiobj->f_onSystemThemeStyleChange();
        }
    }

    const std::wstring& Application::langLocale() const
    {
        return pimpl->uiobj->currLangLocaleName();
    }

    void Application::setLangLocale(const std::wstring& name)
    {
        pimpl->uiobj->changeLangLocale(name);
    }
}
