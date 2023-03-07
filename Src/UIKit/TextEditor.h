#pragma once

#include "Common/Precompile.h"

#include "UIKit/TextInput.h"

namespace d14engine::uikit
{
    struct TextEditor : TextInput
    {
        TextEditor(
            float roundRadius = 0.0f,
            const D2D1_RECT_F& rect = {})
            :
            TextInput(true, roundRadius, rect) { }
    };
}
