#include "Common/Precompile.h"

#include "Common.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/ColorUtils.h"

using namespace d14engine;

namespace d14uikit
{
    Size convert(const D2D1_SIZE_F& size)
    {
        return { math_utils::round(size.width), math_utils::round(size.height) };
    }

    D2D1_SIZE_F convert(const Size& size)
    {
        return { (float)size.width, (float)size.height };
    }

    Point convert(const D2D1_POINT_2F& p)
    {
        return { math_utils::round(p.x), math_utils::round(p.y) };
    }

    D2D1_POINT_2F convert(const Point& p)
    {
        return { (float)p.x, (float)p.y };
    }

    Rect convert(const D2D1_RECT_F& rect)
    {
        return
        {
            math_utils::round(rect.left),  math_utils::round(rect.top),
            math_utils::round(rect.right), math_utils::round(rect.bottom)
        };
    }

    D2D1_RECT_F convert(const Rect& rect)
    {
        return { (float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom };
    }

    Color convert(const D2D1_COLOR_F& cl)
    {
        auto rgb = (uikit::color_utils::iRGB)cl;
        return { rgb.r, rgb.g, rgb.b };
    }

    D2D1_COLOR_F convert(const Color& cl)
    {
        uikit::color_utils::iRGB rgb = { cl.r, cl.g, cl.b };
        return (D2D1_COLOR_F)rgb;
    }
}
