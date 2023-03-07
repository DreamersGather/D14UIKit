#pragma once

#include "Common/Precompile.h"

#include "UIKit/RawTextInput.h"

namespace d14engine::uikit
{
    struct RawTextBox : RawTextInput
    {
        RawTextBox(
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {})
            :
            RawTextInput(false, roundRadius, rect) { }
    };
}
