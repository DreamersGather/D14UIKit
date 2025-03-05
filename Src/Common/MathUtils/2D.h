#pragma once

#include "Common/Precompile.h"

namespace d14engine::math_utils
{
#pragma region Type

    // Usage: Use this structure as a parameter to receive any float2 type value
    // (constexpr can minimize runtime conversion overhead as much as possible):
    //
    // std::pair<float, float> pair = {};
    // D2D1_SIZE_F size = {};
    // D2D1_POINT_2F point = {};
    //
    // void func1(const D2D1_SIZE_F& value);
    // func1({ 1.0f, 2.0f }); --> OK
    // func1(pair); --> Error
    // func1(size); --> OK
    // func1(point); --> Error
    //
    // void func2(const Float2Adapter& value);
    // func2({ 1.0f, 2.0f }); --> OK
    // func2(pair); --> OK
    // func2(size); --> OK
    // func2(point); --> OK

    struct Float2Adapter : std::pair<float, float>
    {
        using pair::pair;

        constexpr Float2Adapter(const D2D1_SIZE_F& rhs)
        {
            first = rhs.width;
            second = rhs.height;
        }
        constexpr Float2Adapter(const D2D1_POINT_2F& rhs)
        {
            first = rhs.x;
            second = rhs.y;
        }
    };
    struct ConstraintResult
    {
        // Whether point overlaps with the target area after being constrained.
        bool isOverlapped = {};
        D2D1_POINT_2F point = {};
    };

#pragma endregion

#pragma region Alias

    using Triangle2D = std::array<D2D1_POINT_2F, 3>;

#pragma endregion

#pragma region Size

    D2D1_SIZE_F ceilf(const D2D1_SIZE_F& size);
    D2D1_SIZE_F floorf(const D2D1_SIZE_F& size);
    D2D1_SIZE_F roundf(const D2D1_SIZE_F& size);

    D2D1_SIZE_U ceilu(const D2D1_SIZE_F& size);
    D2D1_SIZE_U flooru(const D2D1_SIZE_F& size);
    D2D1_SIZE_U roundu(const D2D1_SIZE_F& size);

    D2D1_SIZE_F min(const D2D1_SIZE_F& a, const D2D1_SIZE_F& b);
    D2D1_SIZE_F max(const D2D1_SIZE_F& a, const D2D1_SIZE_F& b);

    D2D1_SIZE_F overrideWidth(const D2D1_SIZE_F& size, float value);
    D2D1_SIZE_F overrideHeight(const D2D1_SIZE_F& size, float value);

    D2D1_SIZE_F increaseWidth(const D2D1_SIZE_F& size, float value);
    D2D1_SIZE_F increaseHeight(const D2D1_SIZE_F& size, float value);

#pragma endregion

#pragma region Point

    D2D1_POINT_2F ceilf(const D2D1_POINT_2F& point);
    D2D1_POINT_2F floorf(const D2D1_POINT_2F& point);
    D2D1_POINT_2F roundf(const D2D1_POINT_2F& point);

    D2D1_POINT_2F minus(const D2D1_POINT_2F& point);
    D2D1_POINT_2F minusX(const D2D1_POINT_2F& point);
    D2D1_POINT_2F minusY(const D2D1_POINT_2F& point);

    D2D1_POINT_2F min(const D2D1_POINT_2F& a, const D2D1_POINT_2F& b);
    D2D1_POINT_2F max(const D2D1_POINT_2F& a, const D2D1_POINT_2F& b);

    D2D1_POINT_2F overrideX(const D2D1_POINT_2F& point, float value);
    D2D1_POINT_2F overrideY(const D2D1_POINT_2F& point, float value);

    D2D1_POINT_2F increaseX(const D2D1_POINT_2F& point, float value);
    D2D1_POINT_2F increaseY(const D2D1_POINT_2F& point, float value);

    D2D1_POINT_2F offset(const D2D1_POINT_2F& point, const Float2Adapter& offset);

#pragma endregion

#pragma region Rectangle

    constexpr D2D1_RECT_F zeroRectF()
    {
        return { 0.0f, 0.0f, 0.0f, 0.0f };
    }
    const D2D1_RECT_F& zeroRectFRef();

    constexpr D2D1_RECT_F infiniteRectF()
    {
        return { -FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX };
    }
    const D2D1_RECT_F& infiniteRectFRef();

    D2D1_RECT_F rect(float left, float top, float width, float height);
    D2D1_RECT_F rect(const D2D1_POINT_2F& leftTop, const D2D1_SIZE_F& size);

    D2D1_RECT_F ceilf(const D2D1_RECT_F& rect);
    D2D1_RECT_F floorf(const D2D1_RECT_F& rect);
    D2D1_RECT_F roundf(const D2D1_RECT_F& rect);

    float width(const D2D1_RECT_F& rect);
    float height(const D2D1_RECT_F& rect);
    D2D1_SIZE_F size(const D2D1_RECT_F& rect);

    D2D1_POINT_2F leftTop(const D2D1_RECT_F& rect);
    D2D1_POINT_2F leftBottom(const D2D1_RECT_F& rect);
    D2D1_POINT_2F rightTop(const D2D1_RECT_F& rect);
    D2D1_POINT_2F rightBottom(const D2D1_RECT_F& rect);

    D2D1_POINT_2F center(const D2D1_RECT_F& rect);
    D2D1_POINT_2F leftCenter(const D2D1_RECT_F& rect);
    D2D1_POINT_2F topCenter(const D2D1_RECT_F& rect);
    D2D1_POINT_2F rightCenter(const D2D1_RECT_F& rect);
    D2D1_POINT_2F bottomCenter(const D2D1_RECT_F& rect);

    D2D1_RECT_F overrideLeft(const D2D1_RECT_F& rect, float value);
    D2D1_RECT_F overrideTop(const D2D1_RECT_F& rect, float value);
    D2D1_RECT_F overrideRight(const D2D1_RECT_F& rect, float value);
    D2D1_RECT_F overrideBottom(const D2D1_RECT_F& rect, float value);

    D2D1_RECT_F overrideLeftRight(const D2D1_RECT_F& rect, const Float2Adapter& f2);
    D2D1_RECT_F overrideTopBottom(const D2D1_RECT_F& rect, const Float2Adapter& f2);

    D2D1_RECT_F increaseLeft(const D2D1_RECT_F& rect, float value);
    D2D1_RECT_F increaseTop(const D2D1_RECT_F& rect, float value);
    D2D1_RECT_F increaseRight(const D2D1_RECT_F& rect, float value);
    D2D1_RECT_F increaseBottom(const D2D1_RECT_F& rect, float value);

    D2D1_RECT_F increaseLeftRight(const D2D1_RECT_F& rect, const Float2Adapter& f2);
    D2D1_RECT_F increaseTopBottom(const D2D1_RECT_F& rect, const Float2Adapter& f2);

    D2D1_RECT_F offset(const D2D1_RECT_F& rect, const Float2Adapter& offset);

    D2D1_RECT_F stretch(const D2D1_RECT_F& rect, const Float2Adapter& extension);

    D2D1_RECT_F moveVertex(const D2D1_RECT_F& rect, const D2D1_RECT_F& delta);

#pragma endregion

#pragma region Miscellaneous

    D2D1_RECT_F widthOnlyRect(float width);
    D2D1_RECT_F heightOnlyRect(float height);
    D2D1_RECT_F sizeOnlyRect(const D2D1_SIZE_F& size);

    D2D1_RECT_F leftBorderRect(const D2D1_RECT_F& rect);
    D2D1_RECT_F topBorderRect(const D2D1_RECT_F& rect);
    D2D1_RECT_F rightBorderRect(const D2D1_RECT_F& rect);
    D2D1_RECT_F bottomBorderRect(const D2D1_RECT_F& rect);

    D2D1_RECT_F adaptMinSize(const D2D1_RECT_F& rect, const D2D1_SIZE_F& size);
    D2D1_RECT_F adaptMaxSize(const D2D1_RECT_F& rect, const D2D1_SIZE_F& size);

    bool isInside(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect);
    bool isOverlapped(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect);

    bool isOverlappedExcludingLeft(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect);
    bool isOverlappedExcludingTop(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect);
    bool isOverlappedExcludingRight(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect);
    bool isOverlappedExcludingBottom(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect);

    D2D1_RECT_F inner(const D2D1_RECT_F& rawFrame, float strokeWidth);
    D2D1_RECT_F outer(const D2D1_RECT_F& rawFrame, float strokeWidth);

    // Example 1: dst = { 0, 0, 10, 10 }, src = { 8, 8 }, return = { 1, 1, 9, 9 }.
    // Example 2: dst = { 2, 3, 18, 15 }, src = { 4, 4 }, return = { 8, 7, 12, 11 }.
    D2D1_RECT_F centered(const D2D1_RECT_F& dst, const D2D1_SIZE_F& src);

    D2D1_POINT_2F constrainTo(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect);

    ConstraintResult constrainToX(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect);
    ConstraintResult constrainToY(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect);

#pragma endregion

}
