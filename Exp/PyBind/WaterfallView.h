#pragma once

#include "Common/Precompile.h"

#include "Common.h"

#include "Callback.h"

#define _D14_UIKIT_PYBIND_WATERFALL_VIEW(Type_Name)                                     \
                                                                                        \
i.def_property_readonly(                                                                \
    "itemCount",                                                                        \
    &Type_Name::itemCount);                                                             \
                                                                                        \
i.def_property_readonly(                                                                \
    "selectedIndicies",                                                                 \
    &Type_Name::selectedIndicies);                                                      \
                                                                                        \
py::enum_<Type_Name::SelectMode>(i, "SelectMode")                                       \
    .value("None", Type_Name::None)                                                     \
    .value("Single", Type_Name::Single)                                                 \
    .value("Multiple", Type_Name::Multiple)                                             \
    .value("Extended", Type_Name::Extended)                                             \
    .export_values();                                                                   \
                                                                                        \
_D14_CALLBACK_PROPERTY(Type_Name, onSelectChange);                                      \
                                                                                        \
i.def("onValueChange", &Ex##Type_Name::onSelectChange, "selected"_a);                   \
