#pragma once

namespace d14uikit
{
    enum class BitmapInterpMode
    {
        NearestNeighbor = 0,
        Linear = 1,
        Cubic = 2,
        MultiSampleLinear = 3,
        Anisotropic = 4,
        HighQualityCubic = 5
    };

    enum class TextAntialiasMode
    {
        Default = 0,
        ClearType = 1,
        Grayscale = 2,
        Aliased = 3
    };

    enum class RenderingMode
    {
        Default = 0,
        Aliased = 1,
        GDIClassic = 2,
        GDINatural = 3,
        Natural = 4,
        NaturalSymmetric = 5,
        Outline = 6
    };

    enum class DrawTextOption
    {
        None = 0, Clip = 2
    };
}
