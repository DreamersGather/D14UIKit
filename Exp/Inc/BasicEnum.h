#pragma once

namespace d14uikit
{
    enum class TextAntialiasMode
    {
        Default = 0,
        ClearType = 1,
        Grayscale = 2,
        Aliased = 3
    };

    enum class D2DRenderingMode
    {
        Default = 0,
        Aliased = 1,
        GDIClassic = 2,
        GDINatural = 3,
        Natural = 4,
        NaturalSymmetric = 5,
        Outline = 6
    };

    enum class BitmapInterpMode
    {
        NearestNeighbor = 0,
        Linear = 1,
        Cubic = 2,
        MultiSampleLinear = 3,
        Anisotropic = 4,
        HighQualityCubic = 5
    };
}
