#include "Common/Precompile.h"

#include "UIKit/AnimationUtils/MotionFunctions.h"

namespace d14engine::uikit::animation_utils
{
    float accelUniformDecelMotion(float dx, float dt, float s, float t1, float t2)
    {
        //       acceleration             uniform              deceleration
        // start -----------> waypoint 1 ----------> waypoint2 -----------> end
        //         motion                  motion                 motion
        // 
        // s --- total distance
        // t1 --- uniform speed motion time
        // t2 --- variable speed motion time
        //
        // v --- uniform speed
        // a --- ac/deceleration
        // 
        // v = s / (t1 + t2)
        // 
        // Then simple to get the distance from start to waypoint 1:
        // 
        // s2 = v * t2 / 2 = s * t2 / (2 * (t1 + t2))
        // 
        // We can calculate the variable speed from current position:
        // 
        // v2 = sqrt((2 * s * dx) / (t2 * (t1 + t2)))
        // 
        // The deceleration motion is similar to the acceleration motion.

        if (dx < s)
        {
            float v = s / (t1 + t2);
            float s2 = v * t2 * 0.5f;

            if (dx < s2)
            {
                // Make the object start moving.
                constexpr float d = 0.1f;
                v = sqrt(2.0f * v * (dx + d) / t2);
            }
            else if (dx > (s - s2))
            {
                v = sqrt(2.0f * v * (s - dx) / t2);
            }
            dx += v * dt;
        }
        return std::clamp(dx, 0.0f, s);
    }
}
