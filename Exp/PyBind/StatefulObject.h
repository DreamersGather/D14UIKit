#pragma once

#include "Common/Precompile.h"

#include "Common.h"

#include "Callback.h"

#define _D14_UIKIT_PYBIND_STATEFUL_OBJECT(Type_Name)                                    \
                                                                                        \
i.def_property("state", &Type_Name::state, &Type_Name::setState);                       \
                                                                                        \
_D14_CALLBACK_PROPERTY(Type_Name, onStateChange);                                       \
                                                                                        \
i.def("onStateChange", &Ex##Type_Name::onStateChange, "state"_a);                       \
