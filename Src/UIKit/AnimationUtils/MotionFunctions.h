#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit::animation_utils
{
    float accelUniformDecelMotion(
        float currDisplacement,
        float lastFrameDeltaSecs,
        float totalDistance,
        float uniformMotionSecs,
        float variableMotionSecs);
}
