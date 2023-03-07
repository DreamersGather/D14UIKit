#pragma once

#include "Common/Precompile.h"

namespace d14engine::cpp_lang_utils
{
    struct NonCopyable
    {
        NonCopyable() = default;

        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };
}
