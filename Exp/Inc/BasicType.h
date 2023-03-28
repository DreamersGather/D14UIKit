#pragma once

namespace d14uikit
{
    struct Point
    {
        int x = {}, y = {};
    };

    struct Size
    {
        int width = {}, height = {};
    };

    struct Rect
    {
        int left = {}, top = {};
        int right = {}, bottom = {};
    };

    struct Color
    {
        int r = {}, g = {}, b = {};
    };

    struct Pixel
    {
        uint8_t b = {}, g = {}, r = {}, a = {};
    };
}
