#pragma once

#include <cstdint>

namespace d14uikit
{
    struct Range
    {
        int offset = {};
        int count = {};
    };

    struct Size
    {
        int width = {};
        int height = {};
    };

    struct Point
    {
        int x = {};
        int y = {};
    };

    struct Rect
    {
        int left = {}, top = {};
        int right = {}, bottom = {};
    };

    struct Color
    {
        int r = {};
        int g = {};
        int b = {};
    };

    struct Pixel
    {
        uint8_t r = {};
        uint8_t g = {};
        uint8_t b = {};
        uint8_t a = {};
    };

    struct ThemeStyle
    {
        std::wstring name = {};
        Color color = {};
    };
}
