#pragma once

#include "Common/Precompile.h"

#include "UIKit/TextInput.h"

namespace d14engine::uikit
{
    struct TextBox : TextInput
    {
        TextBox(
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {})
            :
            TextInput(false, roundRadius, rect) { }
    };
}
