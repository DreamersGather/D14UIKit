#pragma once

#include "Common/Precompile.h"

namespace d14engine::math_utils
{
    constexpr XMFLOAT4X4 identityFloat4x4()
    {
        return
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }
}
