#pragma once

#include "Common/Precompile.h"

#include "Common.h"

#include "Callback.h"
#include "EnumBind.h"

#define _D14_UIKIT_PYBIND_WATERFALL_VIEW(Type_Name)                                     \
                                                                                        \
i.def(py::init());                                                                      \
                                                                                        \
i.def_property_readonly(                                                                \
    "itemCount",                                                                        \
    &Type_Name::itemCount);                                                             \
                                                                                        \
i.def(                                                                                  \
    "getItem",                                                                          \
    &Type_Name::getItem,                                                                \
    "index"_a);                                                                         \
                                                                                        \
i.def_property_readonly(                                                                \
    "selectedIndicies",                                                                 \
    &Type_Name::selectedIndicies);                                                      \
                                                                                        \
py::enum_<Type_Name::SelectMode>(i, "SelectMode")                                       \
    _D14_UIKIT_PYBIND_ENUM(Type_Name, None)                                             \
    _D14_UIKIT_PYBIND_ENUM(Type_Name, Single)                                           \
    _D14_UIKIT_PYBIND_ENUM(Type_Name, Multiple)                                         \
    _D14_UIKIT_PYBIND_ENUM(Type_Name, Extended)                                         \
    .export_values();                                                                   \
                                                                                        \
i.def_property(                                                                         \
    "selectMode",                                                                       \
    &Type_Name::selectMode,                                                             \
    &Type_Name::setSelectMode);                                                         \
                                                                                        \
_D14_CALLBACK_PROPERTY(Type_Name, onSelectChange);                                      \
                                                                                        \
i.def("onSelectChange", &Ex##Type_Name::onSelectChange, "selected"_a);                  \
