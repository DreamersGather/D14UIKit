#pragma once

#include "Common/Precompile.h"

#include "UIKit/RawTextInput.h"

namespace d14engine::uikit
{
    struct RawTextEditor : RawTextInput
    {
        RawTextEditor(
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {})
            :
            RawTextInput(true, roundRadius, rect) { }
    };
}
