#pragma once

#include "Common/Precompile.h"

namespace d14engine::cpp_lang_utils
{
    ///////////////
    // Important //
    ///////////////
    //---------------------------------------------------------------
    // __FUNCSIG__ is a not a standard feature!
    // Different versions of MSVC may have different implementations!
    //---------------------------------------------------------------
    // 
    // enum class Enum { Value1, Value2, Value3 };
    //
    // template<typename Enum_T, Enum_T Value>
    // constexpr void func()
    // {
    //     std::cout << __FUNCSIG__ << std::endl;
    // }
    //---------------------------------------------------------------
    // __FUNCSIG__ results of MSVC:
    //---------------------------------------------------------------
    // 
    // func<int, 1>();
    // "void __cdecl func<int,0x1>(void)"
    //
    // func<Enum, Enum::Value1>();
    // "void __cdecl func<enum Enum,Enum::Value1>(void)"
    //
    // func<Enum, (Enum)99>();
    // "void __cdecl func<enum Enum,(enum Enum)0x63>(void)"

    constexpr size_t g_maxEnumCount = 100;

    constexpr std::wstring_view _d14_invalidEnumName()
    {
        return __FUNCTIONW__;
    }
    constexpr auto g_invalidEnumName = _d14_invalidEnumName();

    ////////////////
    // Enum Check //
    ////////////////
    //
    // enum class Animal { Cat, Dog };
    // 
    // hasEnum<Animal, Animal::Cat> --> true
    // hasEnum<Animal, (Animal)14> --> false

    template<typename Enum_T, Enum_T Value>
    constexpr bool _hasEnum()
    {
        if (std::is_enum_v<Enum_T>)
        {
            constexpr auto name = __FUNCSIG__;
            constexpr auto last = ((int)std::string::traits_type::length(name)) - 1;

            for (auto i = last; i >= 0; --i)
            {
                if (name[i] == ',')
                {
                    return name[i + 1] != '(';
                }
            }
        }
        return false;
    }
    template<typename Enum_T, Enum_T Value>
    constexpr auto hasEnum = _hasEnum<Enum_T, Value>();

    ////////////////
    // Enum Count //
    ////////////////
    //
    // enum class Animal { Cat, Dog };
    // enum class Language { C, Cpp, Python };
    // 
    // enumCount<Animal> --> 2
    // enumCount<Language> --> 3

    template<typename Enum_T>
    constexpr size_t _countEnum()
    {
        return 0;
    }
    template<typename Enum_T, Enum_T Value, Enum_T... Values>
    constexpr size_t _countEnum()
    {
        constexpr auto result = hasEnum<Enum_T, Value>;
        return _countEnum<Enum_T, Values...>() + (size_t)result;
    }
    template<typename Enum_T, size_t... I>
    constexpr size_t _countEnum(std::index_sequence<I...>)
    {
        return _countEnum<Enum_T, (Enum_T)I...>();
    }
    template<typename Enum_T>
    constexpr size_t _enumCount()
    {
        return _countEnum<Enum_T>(std::make_index_sequence<g_maxEnumCount>());
    }
    template<typename Enum_T>
    constexpr auto enumCount = _enumCount<Enum_T>();

    //////////////
    // Enum Map //
    //////////////
    //
    // enum class Animal { Cat, Dog };
    // using Name = std::string;
    // 
    // EnumMap<Animal, Name> names = {};
    // names[(size_t)Animal::Cat] = "happy";
    // names[(size_t)Animal::Dog] = "smile";

    template<typename Key_T, typename Value_T = Key_T>
    using EnumMap = std::array<Value_T, enumCount<Key_T>>;

    ///////////////
    // Enum Name //
    ///////////////
    //
    // enum class Animal { Cat, Dog };
    // 
    // enumName((Animal)0) --> L"Cat"
    // enumName(Animal::Dog) --> L"Dog"
    //
    // Attention! Using enumName to get the string corresponding to an enum value
    // is indeed convenient, but it comes at the cost of runtime performance loss.
    // Below is a benchmark of repeatedly traversing to get strings 10^8 times
    // using enumMap (manually constructed string array) and enumName respectively:
    // (compiled with MSVC Debug/Release flags, running on an AMD 9950X processor):
    //
    // [Debug]              [Release]
    // str: enumMap         str: enumMap
    // time = 3.16469 (s)   time = 0.15178 (s)
    // str: enumName        str: enumName
    // time = 9.37265 (s)   time = 1.26274 (s)
    // 
    // It can be seen that enumName has nearly 3~9 times the performance loss,
    // so you may be careful and do not blindly favoring the elegance of enumName.

    template<typename Enum_T, Enum_T Value>
    constexpr std::wstring_view _enumName()
    {
        if (_hasEnum<Enum_T, Value>())
        {
            constexpr auto name = _CRT_WIDE(__FUNCSIG__);
            constexpr auto last = ((int)std::wstring::traits_type::length(name)) - 1;

            int start = 0, end = 0;
            for (auto i = last; i >= 0; --i)
            {
                if (name[i] == L'>') end = i;
                if (name[i] == L':')
                {
                    start = i + 1;
                    if (end > start)
                    {
                        return { name + start, name + end };
                    }
                }
            }
        }
        return g_invalidEnumName;
    }
    template<typename Enum_T>
    constexpr auto _enumValues()
    {
        constexpr auto count = enumCount<Enum_T>;
        std::array<Enum_T, count> values = {};

#pragma warning(push)
        // Ill-defined for-loop: initial condition does not satisfy test. Loop body not executed.
        // enumCount<Enum_T> is 0 if Enum_T is not an enum type, and it is ok to return directly.
#pragma warning(disable : 6294)
        for (size_t i = 0; i < count; ++i)
        {
            values[i] = (Enum_T)i;
        }
#pragma warning(pop)

        return values;
    }
    template<typename Enum_T, size_t... I>
    constexpr auto _enumNames(std::index_sequence<I...>)
    {
        return std::array<std::wstring_view, sizeof...(I)>
        {
            {_enumName<Enum_T, _enumValues<Enum_T>()[I]>()...}
        };
    }
    template<typename Enum_T>
    constexpr size_t _enumIndex(Enum_T value)
    {
#pragma warning(push)
        // Ill-defined for-loop: initial condition does not satisfy test. Loop body not executed.
        // enumCount<Enum_T> is 0 if Enum_T is not an enum type, and it is ok to return directly.
#pragma warning(disable : 6294)
        for (size_t i = 0; i < enumCount<Enum_T>; ++i)
        {
            if (_enumValues<Enum_T>()[i] == value) return i;
        }
#pragma warning(pop)
        return SIZE_MAX;
    }
    template<typename Enum_T>
    constexpr auto enumName(Enum_T value)
    {
        // We should let the compiler infer it automatically, as value may not be a constexpr;
        // otherwise, using a prefix 'constexpr' here will cause a template compilation error.
        auto index = _enumIndex(value);
        if (index < enumCount<Enum_T>)
        {
            return _enumNames<Enum_T>(std::make_index_sequence<enumCount<Enum_T>>())[index];
        }
        else return g_invalidEnumName;
    }
}
