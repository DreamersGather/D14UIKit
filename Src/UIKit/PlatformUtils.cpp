#include "Common/Precompile.h"

#include "UIKit/PlatformUtils.h"

#include "Common/MathUtils/Basic.h"
#include "Common/MathUtils/GDI.h"

#include "UIKit/Application.h"

namespace d14engine::uikit::platform_utils
{
    float dpi()
    {
        auto app = Application::g_app;
        if (app != nullptr && app->createInfo.dpi.has_value())
        {
            return app->createInfo.dpi.value();
        }
        else return (float)GetSystemDpiForProcess(GetCurrentProcess());
    }

    SIZE scaledByDpi(const SIZE& sz)
    {
        auto factor = dpi() / 96.0f;
        return
        {
            math_utils::round((float)sz.cx * factor),
            math_utils::round((float)sz.cy * factor)
        };
    }

    SIZE restoredByDpi(const SIZE& sz)
    {
        auto factor = 96.0f / dpi();
        return
        {
            math_utils::round((float)sz.cx * factor),
            math_utils::round((float)sz.cy * factor)
        };
    }

    POINT scaledByDpi(const POINT& pt)
    {
        auto factor = dpi() / 96.0f;
        return
        {
            math_utils::round((float)pt.x * factor),
            math_utils::round((float)pt.y * factor)
        };
    }

    POINT restoredByDpi(const POINT& pt)
    {
        auto factor = 96.0f / dpi();
        return
        {
            math_utils::round((float)pt.x * factor),
            math_utils::round((float)pt.y * factor)
        };
    }

    RECT scaledByDpi(const RECT& rc)
    {
        return math_utils::rect
        (
            scaledByDpi(math_utils::leftTop(rc)),
            scaledByDpi(math_utils::size(rc))
        );
    }

    RECT restoredByDpi(const RECT& rc)
    {
        return math_utils::rect
        (
            restoredByDpi(math_utils::leftTop(rc)),
            restoredByDpi(math_utils::size(rc))
        );
    }
}
