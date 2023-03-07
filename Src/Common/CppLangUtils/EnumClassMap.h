#pragma once

#include "Common/Precompile.h"

namespace d14engine::cpp_lang_utils
{
    // If the enum class is defined in the following standard form, then we
    // can use an array to simulate an unordered_map:
    // 
    // enum class XXX { Xxx0, Xxx1, Xxx2, ..., Count };
    // 
    // The structure supports random-access and the lookup will be processed
    // by the compiler since the enum class is static.

    template<typename Key_T, typename Value_T = Key_T>
    using EnumClassMap = std::array<Value_T, (size_t)Key_T::Count>;
}
