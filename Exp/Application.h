#pragma

#include "Common/Precompile.h"

#include "Inc/Application.h"

namespace d14engine::uikit { struct Application; }

namespace d14uikit
{
    struct Application::Impl
    {
        static d14uikit::Application* app;

        std::shared_ptr<d14engine::uikit::Application> uiobj = {};

        bool useSystemTheme = true;

        std::shared_ptr<Cursor> cursor = {};

        bool clearType = false;
        bool drawTextNatrualSymmetric = false;
    };
}
