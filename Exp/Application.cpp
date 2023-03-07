#include "Common/Precompile.h"

#include "Inc/Application.h"

#include "UIKit/Application.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    struct Application::Impl
    {
        static Application* app;
        UniquePtr<uikit::Application> inst = {};

        Impl(int argc, wchar_t* argv[], const CreateInfo& info = {})
        {
            uikit::Application::CreateInfo _info = {};
            _info.name = info.name;
            _info.showCentered = info.centered;
            _info.showMaximized = info.maximized;
            _info.showFullscreen = info.fullscreen;
            if (info.rect.has_value())
            {
                auto& value = info.rect.value();
                _info.win32WindowRect = { value.left, value.top, value.right, value.bottom };
            }
            inst = std::make_unique<uikit::Application>(argc, argv, _info);
        }
    };

    Application* Application::Impl::app = nullptr;

    Application::Application(int argc, wchar_t* argv[], const CreateInfo& info)
        : pimpl(std::make_unique<Impl>(argc, argv, info)) { Impl::app = this; }

    Application::~Application()
    {
        Impl::app = nullptr;

        // fix memory leak error reported by d2d debug layer
        uikit::resource_utils::g_solidColorBrush.Reset();
        uikit::resource_utils::g_shadowEffect.Reset();
    }

    Application* Application::app() { return Impl::app; }

    int Application::run(const std::function<void(Application* app)>& onLaunch)
    {
        return pimpl->inst->run([&](uikit::Application* app) { if (onLaunch) onLaunch(this); });
    }

    void Application::exit() { pimpl->inst->exit(); }
}
