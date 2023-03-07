#include "Common/Precompile.h"

#include "Common/MathUtils/2D.h"

#include "Common/MathUtils/Basic.h"

namespace d14engine::math_utils
{
    D2D1_SIZE_F ceilf(const D2D1_SIZE_F& size)
    {
        return { std::ceil(size.width), std::ceil(size.height) };
    }

    D2D1_SIZE_F floorf(const D2D1_SIZE_F& size)
    {
        return { std::floor(size.width), std::floor(size.height) };
    }

    D2D1_SIZE_F roundf(const D2D1_SIZE_F& size)
    {
        return { std::round(size.width), std::round(size.height) };
    }

    D2D1_SIZE_U ceilu(const D2D1_SIZE_F& size)
    {
        return { ceil<UINT32>(size.width), ceil<UINT32>(size.height) };
    }

    D2D1_SIZE_U flooru(const D2D1_SIZE_F& size)
    {
        return { floor<UINT32>(size.width), floor<UINT32>(size.height) };
    }

    D2D1_SIZE_U roundu(const D2D1_SIZE_F& size)
    {
        return { round<UINT32>(size.width), round<UINT32>(size.height) };
    }

    D2D1_SIZE_F min(const D2D1_SIZE_F& a, const D2D1_SIZE_F& b)
    {
        return { std::min(a.width, b.width), std::min(a.height, b.height) };
    }

    D2D1_SIZE_F max(const D2D1_SIZE_F& a, const D2D1_SIZE_F& b)
    {
        return { std::max(a.width, b.width), std::max(a.height, b.height) };
    }

    D2D1_SIZE_F overrideWidth(const D2D1_SIZE_F& size, float value)
    {
        return { value, size.height };
    }

    D2D1_SIZE_F overrideHeight(const D2D1_SIZE_F& size, float value)
    {
        return { size.width, value };
    }

    D2D1_SIZE_F increaseWidth(const D2D1_SIZE_F& size, float value)
    {
        return { size.width + value, size.height };
    }

    D2D1_SIZE_F increaseHeight(const D2D1_SIZE_F& size, float value)
    {
        return { size.width, size.height + value };
    }

    D2D1_POINT_2F ceilf(const D2D1_POINT_2F& point)
    {
        return { std::ceil(point.x), std::ceil(point.y) };
    }

    D2D1_POINT_2F floorf(const D2D1_POINT_2F& point)
    {
        return { std::floor(point.x), std::floor(point.y) };
    }

    D2D1_POINT_2F roundf(const D2D1_POINT_2F& point)
    {
        return { std::round(point.x), std::round(point.y) };
    }

    D2D1_POINT_2F minus(const D2D1_POINT_2F& point)
    {
        return { -point.x, -point.y };
    }

    D2D1_POINT_2F minusX(const D2D1_POINT_2F& point)
    {
        return { -point.x, point.y };
    }

    D2D1_POINT_2F minusY(const D2D1_POINT_2F& point)
    {
        return { point.x, -point.y };
    }

    D2D1_POINT_2F min(const D2D1_POINT_2F& a, const D2D1_POINT_2F& b)
    {
        return { std::min(a.x, b.x), std::min(a.y, b.y) };
    }

    D2D1_POINT_2F max(const D2D1_POINT_2F& a, const D2D1_POINT_2F& b)
    {
        return { std::max(a.x, b.x), std::max(a.y, b.y) };
    }

    D2D1_POINT_2F overrideX(const D2D1_POINT_2F& point, float value)
    {
        return { value, point.y };
    }

    D2D1_POINT_2F overrideY(const D2D1_POINT_2F& point, float value)
    {
        return { point.x, value };
    }

    D2D1_POINT_2F increaseX(const D2D1_POINT_2F& point, float value)
    {
        return { point.x + value, point.y };
    }

    D2D1_POINT_2F increaseY(const D2D1_POINT_2F& point, float value)
    {
        return { point.x, point.y + value };
    }

    D2D1_POINT_2F offset(const D2D1_POINT_2F& point, const Float2Adapter& offset)
    {
        return { point.x + offset.first, point.y + offset.second };
    }

    const static D2D1_RECT_F g_zeroRectF =
    {
        0.0f, 0.0f, 0.0f, 0.0f
    };
    const D2D1_RECT_F& zeroRectFRef()
    {
        return g_zeroRectF;
    }

    const static D2D1_RECT_F g_infiniteRectF =
    {
        -FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX
    };
    const D2D1_RECT_F& infiniteRectFRef()
    {
        return g_infiniteRectF;
    }

    D2D1_RECT_F rect(float left, float top, float width, float height)
    {
        return { left, top, left + width, top + height };
    }

    D2D1_RECT_F rect(const D2D1_POINT_2F& leftTop, const D2D1_SIZE_F& size)
    {
        return rect(leftTop.x, leftTop.y, size.width, size.height);
    }

    D2D1_RECT_F ceilf(const D2D1_RECT_F& rect)
    {
        return { std::ceil(rect.left), std::ceil(rect.top), std::ceil(rect.right), std::ceil(rect.bottom) };
    }

    D2D1_RECT_F floorf(const D2D1_RECT_F& rect)
    {
        return { std::floor(rect.left), std::floor(rect.top), std::floor(rect.right), std::floor(rect.bottom) };
    }

    D2D1_RECT_F roundf(const D2D1_RECT_F& rect)
    {
        return { std::round(rect.left), std::round(rect.top), std::round(rect.right), std::round(rect.bottom) };
    }

    float width(const D2D1_RECT_F& rect)
    {
        return rect.right - rect.left;
    }

    float height(const D2D1_RECT_F& rect)
    {
        return rect.bottom - rect.top;
    }

    D2D1_SIZE_F size(const D2D1_RECT_F& rect)
    {
        return { width(rect), height(rect) };
    }

    D2D1_POINT_2F leftTop(const D2D1_RECT_F& rect)
    {
        return { rect.left, rect.top };
    }

    D2D1_POINT_2F leftBottom(const D2D1_RECT_F& rect)
    {
        return { rect.left, rect.bottom };
    }

    D2D1_POINT_2F rightTop(const D2D1_RECT_F& rect)
    {
        return { rect.right, rect.top };
    }

    D2D1_POINT_2F rightBottom(const D2D1_RECT_F& rect)
    {
        return { rect.right, rect.bottom };
    }

    D2D1_POINT_2F center(const D2D1_RECT_F& rect)
    {
        return { (rect.left + rect.right) * 0.5f, (rect.top + rect.bottom) * 0.5f };
    }

    D2D1_POINT_2F leftCenter(const D2D1_RECT_F& rect)
    {
        return { rect.left, (rect.top + rect.bottom) * 0.5f };
    }

    D2D1_POINT_2F topCenter(const D2D1_RECT_F& rect)
    {
        return { (rect.left + rect.right) * 0.5f, rect.top };
    }

    D2D1_POINT_2F rightCenter(const D2D1_RECT_F& rect)
    {
        return { rect.right, (rect.top + rect.bottom) * 0.5f };
    }

    D2D1_POINT_2F bottomCenter(const D2D1_RECT_F& rect)
    {
        return { (rect.left + rect.right) * 0.5f, rect.bottom };
    }

    D2D1_RECT_F overrideLeft(const D2D1_RECT_F& rect, float value)
    {
        return { value, rect.top, rect.right, rect.bottom };
    }

    D2D1_RECT_F overrideTop(const D2D1_RECT_F& rect, float value)
    {
        return { rect.left, value, rect.right, rect.bottom };
    }

    D2D1_RECT_F overrideRight(const D2D1_RECT_F& rect, float value)
    {
        return { rect.left, rect.top, value, rect.bottom };
    }

    D2D1_RECT_F overrideBottom(const D2D1_RECT_F& rect, float value)
    {
        return { rect.left, rect.top, rect.right, value };
    }

    D2D1_RECT_F overrideLeftRight(const D2D1_RECT_F& rect, const OffsetSpanF& span)
    {
        return { span.start, rect.top, span.end, rect.bottom };
    }

    D2D1_RECT_F overrideTopBottom(const D2D1_RECT_F& rect, const OffsetSpanF& span)
    {
        return { rect.left, span.start, rect.right, span.end };
    }

    D2D1_RECT_F increaseLeft(const D2D1_RECT_F& rect, float value)
    {
        return { rect.left + value, rect.top, rect.right, rect.bottom };
    }

    D2D1_RECT_F increaseTop(const D2D1_RECT_F& rect, float value)
    {
        return { rect.left, rect.top + value, rect.right, rect.bottom };
    }

    D2D1_RECT_F increaseRight(const D2D1_RECT_F& rect, float value)
    {
        return { rect.left, rect.top, rect.right + value, rect.bottom };
    }

    D2D1_RECT_F increaseBottom(const D2D1_RECT_F& rect, float value)
    {
        return { rect.left, rect.top, rect.right, rect.bottom + value };
    }

    D2D1_RECT_F increaseLeftRight(const D2D1_RECT_F& rect, const OffsetSpanF& span)
    {
        return { rect.left + span.start, rect.top, rect.right + span.end, rect.bottom };
    }

    D2D1_RECT_F increaseTopBottom(const D2D1_RECT_F& rect, const OffsetSpanF& span)
    {
        return { rect.left, rect.top + span.start, rect.right, rect.bottom + span.end };
    }

    D2D1_RECT_F stretch(const D2D1_RECT_F& rect, const Float2Adapter& extension)
    {
        return { rect.left - extension.first, rect.top - extension.second, rect.right + extension.first, rect.bottom + extension.second };
    }

    D2D1_RECT_F offset(const D2D1_RECT_F& rect, const Float2Adapter& offset)
    {
        return { rect.left + offset.first, rect.top + offset.second, rect.right + offset.first, rect.bottom + offset.second };
    }

    D2D1_RECT_F moveVertex(const D2D1_RECT_F& rect, const D2D1_RECT_F& delta)
    {
        return { rect.left + delta.left, rect.top + delta.top, rect.right + delta.right, rect.bottom + delta.bottom };
    }

    D2D1_RECT_F widthOnlyRect(float width)
    {
        return { 0.0f, 0.0f, width, 0.0f };
    }

    D2D1_RECT_F heightOnlyRect(float height)
    {
        return { 0.0f, 0.0f, 0.0f, height };
    }

    D2D1_RECT_F sizeOnlyRect(const D2D1_SIZE_F& size)
    {
        return { 0.0f, 0.0f, size.width, size.height };
    }

    D2D1_RECT_F leftBorderRect(const D2D1_RECT_F& rect)
    {
        return { rect.left, rect.top, rect.left, rect.bottom };
    }

    D2D1_RECT_F topBorderRect(const D2D1_RECT_F& rect)
    {
        return { rect.left, rect.top, rect.right, rect.top };
    }

    D2D1_RECT_F rightBorderRect(const D2D1_RECT_F& rect)
    {
        return { rect.right, rect.top, rect.right, rect.bottom };
    }

    D2D1_RECT_F bottomBorderRect(const D2D1_RECT_F& rect)
    {
        return { rect.left, rect.bottom, rect.right, rect.bottom };
    }

    D2D1_RECT_F adaptMinSize(const D2D1_RECT_F& rect, const D2D1_SIZE_F& size)
    {
        return math_utils::rect(leftTop(rect), min(math_utils::size(rect), size));
    }

    D2D1_RECT_F adaptMaxSize(const D2D1_RECT_F& rect, const D2D1_SIZE_F& size)
    {
        return math_utils::rect(leftTop(rect), max(math_utils::size(rect), size));
    }

    bool isInside(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect)
    {
        return point.x > rect.left && point.x < rect.right&& point.y > rect.top && point.y < rect.bottom;
    }

    bool isOverlapped(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect)
    {
        return point.x >= rect.left && point.x <= rect.right && point.y >= rect.top && point.y <= rect.bottom;
    }

    bool isOverlappedExcludingLeft(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect)
    {
        return isOverlapped(point, rect) && point.x != rect.left;
    }

    bool isOverlappedExcludingTop(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect)
    {
        return isOverlapped(point, rect) && point.y != rect.top;
    }

    bool isOverlappedExcludingRight(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect)
    {
        return isOverlapped(point, rect) && point.x != rect.right;
    }

    bool isOverlappedExcludingBottom(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect)
    {
        return isOverlapped(point, rect) && point.y != rect.bottom;
    }

    D2D1_RECT_F inner(const D2D1_RECT_F& rawFrame, float strokeWidth)
    {
        return stretch(rawFrame, { -strokeWidth * 0.5f, -strokeWidth * 0.5f });
    }

    D2D1_RECT_F outer(const D2D1_RECT_F& rawFrame, float strokeWidth)
    {
        return stretch(rawFrame, { +strokeWidth * 0.5f, +strokeWidth * 0.5f });
    }

    D2D1_RECT_F centered(const D2D1_RECT_F& dst, const D2D1_SIZE_F& src)
    {
        return rect(dst.left + (width(dst) - src.width) * 0.5f, dst.top + (height(dst) - src.height) * 0.5f, src.width, src.height);
    }

    D2D1_POINT_2F constrainTo(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect)
    {
        return { std::clamp(point.x, rect.left, rect.right), std::clamp(point.y, rect.top, rect.bottom) };
    }

    ConstraintResult constrainToX(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect)
    {
        return { point.y >= rect.top && point.y <= rect.bottom, { std::clamp(point.x, rect.left, rect.right), point.y } };
    }

    ConstraintResult constrainToY(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect)
    {
        return { point.x >= rect.left && point.x <= rect.right, { point.x, std::clamp(point.y, rect.top, rect.bottom) } };
    }
}
