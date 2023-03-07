#pragma once

#include "Common/Precompile.h"

namespace d14engine::cpp_lang_utils
{
    template<typename, template<typename...> typename>
    constexpr bool isInstanceOf = false;

    template<typename... Args, template<typename...> typename Temp>
    constexpr bool isInstanceOf<Temp<Args...>, Temp> = true;

    namespace has_operator
    {
        struct Void { };

        // We must enclose the declarations of these dummy operators into a
        // namespace to prevent them from messing with other codes.

        template<typename T, typename Arg>
        Void operator<(const T&, const Arg&);

        template<typename T, typename Arg = T>
        constexpr bool lessThan = !std::is_same_v<decltype(std::declval<T>() < std::declval<Arg>()), Void>;

        template<typename T, typename Arg>
        Void operator==(const T&, const Arg&);

        template<typename T, typename Arg = T>
        constexpr bool equalTo = !std::is_same_v<decltype(std::declval<T>() == std::declval<Arg>()), Void>;
    }
}
