#pragma once

#include "Common/Precompile.h"

namespace d14engine::math_utils
{
    template<typename RT = int, typename T>
    RT ceil(T value) { return (RT)std::ceil(value); }

    template<typename RT = int, typename T>
    RT floor(T value) { return (RT)std::floor(value); }

    template<typename RT = int, typename T>
    RT round(T value) { return (RT)std::round(value); }
}
