#pragma once

#include "Common/Precompile.h"

namespace d14engine::cpp_lang_utils
{
    // We can get the address of the most-derived object by casting void-ptr,
    // which is very helpful for the comparison in multiple inheritance.
    //
    // Besides, it is recommended to use dynamic_cast to be compatible with
    // the potential virtual inheritance at the tolerable cost of speed.

    template<typename T, typename U>
    bool isMostDerivedEqual(T* lhs, U* rhs)
    {
        return dynamic_cast<const void*>(lhs) == dynamic_cast<const void*>(rhs);
    }

    template<typename T, typename U>
    bool isMostDerivedEqual(ShrdPtrParam<T> lhs, ShrdPtrParam<U> rhs)
    {
        return std::dynamic_pointer_cast<const void>(lhs) == std::dynamic_pointer_cast<const void>(rhs);
    }
}
