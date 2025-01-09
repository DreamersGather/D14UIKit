#pragma

#include "Common/Precompile.h"

#include "Inc/Application.h"

namespace d14uikit
{
    struct Application::Impl
    {
        static d14uikit::Application* app;

        std::shared_ptr<d14engine::uikit::Application> uiobj = {};

        bool animState = false;

        std::shared_ptr<Cursor> cursor = {};

        bool useSystemTheme = true;

        BitmapInterpMode bitmapInterpMode = BitmapInterpMode::Linear;

        TextAntialiasMode textAntialiasMode = TextAntialiasMode::Default;
        RenderingMode renderingMode = RenderingMode::Default;
    };
}
