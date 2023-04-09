#pragma once

#include "Common/Precompile.h"

#include "Inc/BasicType.h"

namespace d14uikit
{
    Size convert(const D2D1_SIZE_F& size);
    D2D1_SIZE_F convert(const Size& size);

    Point convert(const D2D1_POINT_2F& p);
    D2D1_POINT_2F convert(const Point& p);

    Rect convert(const D2D1_RECT_F& rect);
    D2D1_RECT_F convert(const Rect& rect);

    Color convert(const D2D1_COLOR_F& cl);
    D2D1_COLOR_F convert(const Color& cl);
}
