#pragma once

#include "Common/Precompile.h"

#include "Common.h"

#include "Callback.h"
#include "EnumBind.h"

#include "Inc/Label.h"

#define _D14_UIKIT_PYBIND_SLIDER(Type_Name)                                             \
                                                                                        \
i.def(py::init());                                                                      \
                                                                                        \
i.def_property(                                                                         \
    "value",                                                                            \
    &Type_Name::value,                                                                  \
    &Type_Name::setValue);                                                              \
                                                                                        \
i.def_property(                                                                         \
    "minValue",                                                                         \
    &Type_Name::minValue,                                                               \
    &Type_Name::setMinValue);                                                           \
                                                                                        \
i.def_property(                                                                         \
    "maxValue",                                                                         \
    &Type_Name::maxValue,                                                               \
    &Type_Name::setMaxValue);                                                           \
                                                                                        \
py::enum_<Type_Name::StepMode>(i, "StepMode")                                           \
    _D14_UIKIT_PYBIND_ENUM(Type_Name, Continuous)                                       \
    _D14_UIKIT_PYBIND_ENUM(Type_Name, Discrete)                                         \
    .export_values();                                                                   \
                                                                                        \
i.def_property(                                                                         \
    "stepMode",                                                                         \
    &Type_Name::stepMode,                                                               \
    &Type_Name::setStepMode);                                                           \
                                                                                        \
i.def_property(                                                                         \
    "stepInterval",                                                                     \
    &Type_Name::stepInterval,                                                           \
    &Type_Name::setStepInterval);                                                       \
                                                                                        \
i.def_property_readonly(                                                                \
    "valueLabel",                                                                       \
    &Type_Name::valueLabel);                                                            \
                                                                                        \
i.def_property(                                                                         \
    "vlabelEnabled",                                                                    \
    &Type_Name::vlabelEnabled,                                                          \
    &Type_Name::setVlabelEnabled);                                                      \
                                                                                        \
i.def_property(                                                                         \
    "vlabelPrecision",                                                                  \
    &Type_Name::vlabelPrecision,                                                        \
    &Type_Name::setVlabelPrecision);                                                    \
                                                                                        \
i.def_property(                                                                         \
    "vlabelResident",                                                                   \
    &Type_Name::vlabelResident,                                                         \
    &Type_Name::setVlabelResident);                                                     \
                                                                                        \
i.def_property(                                                                         \
    "vlabelOffset",                                                                     \
    &Type_Name::vlabelOffset,                                                           \
    &Type_Name::setVlabelOffset);                                                       \
                                                                                        \
i.def_property(                                                                         \
    "vlabelRoundRaidus",                                                                \
    &Type_Name::vlabelRoundRaidus,                                                      \
    &Type_Name::setVlabelRoundRaidus);                                                  \
                                                                                        \
i.def_property(                                                                         \
    "vlabelRectSize",                                                                   \
    &Type_Name::vlabelRectSize,                                                         \
    &Type_Name::setVlabelRectSize);                                                     \
                                                                                        \
i.def_property(                                                                         \
    "vlabelTipSize",                                                                    \
    &Type_Name::vlabelTipSize,                                                          \
    &Type_Name::setVlabelTipSize);                                                      \
                                                                                        \
_D14_CALLBACK_PROPERTY(Type_Name, onValueChange);                                       \
_D14_CALLBACK_PROPERTY(Type_Name, onEndSliding);                                        \
_D14_CALLBACK_PROPERTY(Type_Name, onStartSliding);                                      \
                                                                                        \
i.def("onValueChange", &Ex##Type_Name::onValueChange, "value"_a);                       \
                                                                                        \
i.def("onEndSliding", &Ex##Type_Name::onEndSliding, "value"_a);                         \
                                                                                        \
i.def("onStartSliding", &Ex##Type_Name::onStartSliding, "value"_a);                     \
