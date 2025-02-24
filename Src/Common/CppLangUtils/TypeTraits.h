#pragma once

#include "Common/Precompile.h"

namespace d14engine::cpp_lang_utils
{
    // Check whether the type is instantiated from the specific template:
    // 
    // using IntArray = std::vector<int>;
    //
    // constexpr bool value1 = isInstanceOf<IntArray, std::vector>; // true
    // constexpr bool value2 = isInstanceOf<IntArray, std::list>; // false

    template<typename, template<typename...> typename>
    constexpr bool isInstanceOf = false;

    template<typename... Args, template<typename...> typename Temp>
    constexpr bool isInstanceOf<Temp<Args...>, Temp> = true;

    // Check whether the type has definitions of the specific operators:
    //
    // constexpr bool value1 = has_operator::equalTo<int>; // true
    // constexpr bool value2 = has_operator::lessThan<int>; // true
    // 
    // struct A
    // {
    //     int a = 0;
    //
    //     bool operator==(const A & rhs) const
    //     { return a == rhs.a; }
    // };
    //
    // constexpr bool value1 = has_operator::equalTo<A>; // true
    // constexpr bool value2 = has_operator::lessThan<A>; // false

    namespace has_operator
    {
        struct Void { };

        // We must enclose the declarations of these dummy operators into
        // a namespace to prevent them from messing with other codes.

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
