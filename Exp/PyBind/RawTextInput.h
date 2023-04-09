#pragma once

#include "Common/Precompile.h"

#include "Common.h"

#include "Callback.h"

#include "Inc/Label.h"

#define _D14_UIKIT_PYBIND_RAW_TEXT_INPUT(Type_Name)                                     \
                                                                                        \
i.def_property("textRect", &Type_Name::textRect, &Type_Name::setTextRect);              \
                                                                                        \
i.def_property_readonly("placeholder", &Type_Name::placeholder);                        \
                                                                                        \
i.def("performCommandCtrlX", &Type_Name::performCommandCtrlX);                          \
                                                                                        \
i.def("performCommandCtrlV", &Type_Name::performCommandCtrlV);                          \
                                                                                        \
_D14_CALLBACK_PROPERTY(Type_Name, onTextChange);                                        \
                                                                                        \
i.def("onTextChange", &Ex##Type_Name::onTextChange, "text"_a);                          \
