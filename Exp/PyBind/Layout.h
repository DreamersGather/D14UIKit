#pragma once

#include "Common/Precompile.h"

#include "Common.h"

#define _D14_UIKIT_PYBIND_LAYOUT(Type_Name)                                             \
                                                                                        \
i.def(py::init());                                                                      \
                                                                                        \
i.def_property(                                                                         \
    "opacity",                                                                          \
    & Type_Name::opacity,                                                               \
    & Type_Name::setOpacity);                                                           \
                                                                                        \
i.def_property(                                                                         \
    "outlineWidth",                                                                     \
    &Type_Name::outlineWidth,                                                           \
    &Type_Name::setOutlineWidth);                                                       \
                                                                                        \
i.def_property(                                                                         \
    "outlineOpacity",                                                                   \
    &Type_Name::outlineOpacity,                                                         \
    &Type_Name::setOutlineOpacity);                                                     \
                                                                                        \
i.def(                                                                                  \
    "addElement",                                                                       \
    &Type_Name::addElement);                                                            \
                                                                                        \
i.def(                                                                                  \
    "removeElement",                                                                    \
    &Type_Name::removeElement);                                                         \
                                                                                        \
i.def(                                                                                  \
    "clearAllElements",                                                                 \
    &Type_Name::clearAllElements);                                                      \
                                                                                        \
i.def(                                                                                  \
    "updateElement",                                                                    \
    &Type_Name::updateElement);                                                         \
