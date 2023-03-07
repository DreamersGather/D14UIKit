#include "Common/Precompile.h"

#include "Inc/Application.h"

#include "UIKit/Application.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    struct Application::Impl : uikit::Application
    {
        using Application::Application;

        static d14uikit::Application* app;
    };

    Application::Application(int argc, wchar_t* argv[], const CreateInfo& info)
    {
        Impl::app = this;

        uikit::Application::CreateInfo info2 = {};
        info2.name = info.name;
        info2.showCentered = info.centered;
        info2.showMaximized = info.maximized;
        info2.showFullscreen = info.fullscreen;
        if (info.rect.has_value())
        {
            auto& rect = info.rect.value();
            info2.win32WindowRect =
            {
                rect.left, rect.top,
                rect.right, rect.bottom
            };
        }
        pimpl = std::make_shared<Impl>(argc, argv, info2);
    }

    Application::~Application()
    {
        Impl::app = nullptr;

        // fix memory leak error reported by d2d debug layer
        uikit::resource_utils::g_solidColorBrush.Reset();
        uikit::resource_utils::g_shadowEffect.Reset();
    }

    Application* Application::Impl::app = nullptr;

    Application* Application::app() { return Impl::app; }

    int Application::run(const std::function<void(Application* app)>& onLaunch)
    {
        return pimpl->run([&](uikit::Application* app) { if (onLaunch) onLaunch(this); });
    }

    void Application::exit() { pimpl->exit(); }
}
