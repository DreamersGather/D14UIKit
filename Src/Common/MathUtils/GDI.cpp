#include "Common/Precompile.h"

#include "Common/MathUtils/GDI.h"

namespace d14engine::math_utils
{
    const static RECT g_zeroRect =
    {
        0, 0, 0, 0
    };

    const RECT& zeroRectRef()
    {
        return g_zeroRect;
    }

    RECT rect(LONG left, LONG top, LONG width, LONG height)
    {
        return { left, top, left + width, top + height };
    }

    RECT rect(const POINT& leftTop, const SIZE& size)
    {
        return rect(leftTop.x, leftTop.y, size.cx, size.cy);
    }

    LONG width(const RECT& rect)
    {
        return rect.right - rect.left;
    }

    LONG height(const RECT& rect)
    {
        return rect.bottom - rect.top;
    }

    SIZE size(const RECT& rect)
    {
        return { width(rect), height(rect) };
    }

    POINT center(const RECT& rect)
    {
        return { (rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2 };
    }

    POINT leftTop(const RECT& rect)
    {
        return { rect.left, rect.top };
    }

    POINT leftBottom(const RECT& rect)
    {
        return { rect.left, rect.bottom };
    }

    POINT rightTop(const RECT& rect)
    {
        return { rect.right, rect.top };
    }

    POINT rightBottom(const RECT& rect)
    {
        return { rect.right, rect.bottom };
    }

    RECT centered(const RECT& dst, const SIZE& src)
    {
        return rect(dst.left + (width(dst) - src.cx) / 2, dst.top + (height(dst) - src.cy) / 2, src.cx, src.cy);
    }
}
