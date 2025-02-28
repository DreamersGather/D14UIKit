#pragma once

#include "Common/Precompile.h"

#include "Common.h"

// Usage:
// py::enum_<Class_Name::Type_Name>(i, "Type_Name")
//     _D14_UIKIT_PYBIND_ENUM(Type_Name, Object_Name_0)
//     _D14_UIKIT_PYBIND_ENUM(Type_Name, Object_Name_1)
//     _D14_UIKIT_PYBIND_ENUM(Type_Name, Object_Name_2)
//     .export_values();

// As you can see, even with macros, the enum binding is still quite cumbersome.
// We have tried using macros and metaprogramming to achieve list expansion,
// hoping to achieve the above effect with a macro like Macro(a, b0, b1, b2).
// However, this approach had poor portability, so we eventually abandoned it.

#define _D14_UIKIT_PYBIND_ENUM(Type_Name, Object_Name) \
.value(#Object_Name, Type_Name::Object_Name)
