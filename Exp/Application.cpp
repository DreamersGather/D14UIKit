#include "Common/Precompile.h"

#include "Cursor.h"
#include "Panel.h"
#include "Application.h"

#include "UIKit/Cursor.h"
#include "Renderer/TickTimer.h"
#include "UIKit/Application.h"
#include "UIKit/ColorUtils.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    Application* Application::Impl::app = nullptr;

    Application::Application(const std::wstring& name)
        :
        Application(Passkey{})
    {
        pimpl->uiobj = std::make_shared<uikit::Application>(
            0, nullptr, uikit::Application::CreateInfo{ .name = name });

        initialize();
    }

    Application::Application(Passkey)
        :
        pimpl(std::make_shared<Impl>())
    {
        Impl::app = this;
    }

    Application::~Application()
    {
        Impl::app = nullptr;

        // fix memory leak reported by d2d debug layer
        uikit::resource_utils::g_solidColorBrush.Reset();
        uikit::resource_utils::g_shadowEffect.Reset();
    }

    void Application::initialize()
    {
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
        // Since Cursor::Passkey is protected, std::make_shared will fail here.
        pimpl->cursor = std::shared_ptr<Cursor>(new Cursor(Cursor::Passkey{}));
        
        auto& cpimpl = pimpl->cursor->Panel::pimpl;
        auto& ccimpl = pimpl->cursor->Cursor::pimpl;

        auto cpuiobj = pimpl->uiobj->cursor()->shared_from_this();
        auto ccuiobj = std::static_pointer_cast<uikit::Cursor>(cpuiobj);

        cpimpl->uiobj = cpuiobj;
        ccimpl->uiobj = ccuiobj;
        
        pimpl->cursor->Panel::initialize();
        pimpl->cursor->Cursor::initialize();
    }

    Application* Application::app()
    {
        return Impl::app;
    }

    Cursor* Application::cursor() const
    {
        return pimpl->cursor.get();
    }

    int Application::run()
    {
        return pimpl->uiobj->run();
    }

    void Application::exit()
    {
        pimpl->uiobj->exit();
    }

    Size Application::size() const
    {
        RECT rc = {};
        GetClientRect(pimpl->uiobj->win32Window(), &rc);
        return { rc.right - rc.left, rc.bottom - rc.top };
    }

    void Application::setSize(const Size& value)
    {
        SetWindowPos(
            pimpl->uiobj->win32Window(),
            HWND_TOPMOST,
            0, // nomove
            0, // nomove
            value.width,
            value.height,
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
        return { rc.left, rc.top };
    }

    void Application::setPosition(const Point& value)
    {
        SetWindowPos(
            pimpl->uiobj->win32Window(),
            HWND_TOPMOST,
            value.x,
            value.y,
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

    bool Application::fullscreen() const
    {
        return pimpl->uiobj->dxRenderer()->window().fullscreen();
    }

    void Application::setFullscreen(bool value)
    {
        auto& dxwnd = pimpl->uiobj->dxRenderer()->window();
        if (value) dxwnd.enterFullscreenMode();
        else dxwnd.restoreWindowedMode();
    }

    int Application::fps() const
    {
        return pimpl->uiobj->dxRenderer()->timer()->fps();
    }

    bool Application::lowEnergy() const
    {
        return pimpl->uiobj->animationCount() <= 0;
    }

    void Application::setLowEnergy(bool value)
    {
        if (value) pimpl->uiobj->decreaseAnimationCount();
        else pimpl->uiobj->increaseAnimationCount();
    }

    const std::wstring& Application::themeMode() const
    {
        return pimpl->uiobj->currThemeName();
    }

    void Application::setThemeMode(const std::wstring& name)
    {
        if (name != L"Light" || name != L"Dark") return;

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

        auto rgb =  uikit::color_utils::convert(style.color);
        return { rgb.R, rgb.G, rgb.B };
    }

    void Application::setThemeColor(const Color& value)
    {
        if (useSystemTheme()) setUseSystemTheme(false);

        auto& style = pimpl->uiobj->customThemeStyle.value();

        style.color = uikit::color_utils::convert(
            uikit::color_utils::iRGB{ value.r, value.g, value.b });

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
        pimpl->useSystemTheme = value;
        if (pimpl->useSystemTheme)
        {
            pimpl->uiobj->changeTheme(pimpl->uiobj->currThemeName());
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
