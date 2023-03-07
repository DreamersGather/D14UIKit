#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit
{
    struct SolidStyle
    {
        D2D1_COLOR_F color = D2D1::ColorF{ 0x000000 };
        float opacity = 1.0f;
    };
}
