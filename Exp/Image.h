#pragma once

#include "Inc/Image.h"

namespace d14uikit
{
    struct Image::Impl
    {
        d14engine::ComPtr<ID2D1Bitmap1> bitmap = {};

        bool cpuRead = {}; // Can map to CPU side?
    };
}
