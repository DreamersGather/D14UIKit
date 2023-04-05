#pragma once

#include "Common/Precompile.h"

#include "Common.h"

#define _D14_UIKIT_PYBIND_TEXT_FORMAT(Type_Name, Suffix)  \
                                                          \
i.def_property(                                           \
    "font" #Suffix,                                       \
    &Type_Name::font##Suffix,                             \
    &Type_Name::setFont##Suffix);                         \
                                                          \
i.def_property(                                           \
    "fontFamilyName" #Suffix,                             \
    &Type_Name::fontFamilyName##Suffix,                   \
    &Type_Name::setFontFamilyName##Suffix);               \
                                                          \
i.def_property(                                           \
    "fontSize" #Suffix,                                   \
    &Type_Name::fontSize##Suffix,                         \
    &Type_Name::setFontSize##Suffix);                     \
                                                          \
i.def_property(                                           \
    "fontLocaleName" #Suffix,                             \
    &Type_Name::fontLocaleName##Suffix,                   \
    &Type_Name::setFontLocaleName##Suffix);               \
                                                          \
i.def_property(                                           \
    "fontWeight" #Suffix,                                 \
    &Type_Name::fontWeight##Suffix,                       \
    &Type_Name::setFontWeight##Suffix);                   \
                                                          \
i.def_property(                                           \
    "fontStyle" #Suffix,                                  \
    &Type_Name::fontStyle##Suffix,                        \
    &Type_Name::setFontStyle##Suffix);                    \
                                                          \
i.def_property(                                           \
    "fontStretch" #Suffix,                                \
    &Type_Name::fontStretch##Suffix,                      \
    &Type_Name::setFontStretch##Suffix);                  \
                                                          \
i.def_property(                                           \
    "underline" #Suffix,                                  \
    &Type_Name::underline##Suffix,                        \
    &Type_Name::setUnderline##Suffix);                    \
                                                          \
i.def_property(                                           \
    "strikethrough" #Suffix,                              \
    &Type_Name::strikethrough##Suffix,                    \
    &Type_Name::setStrikethrough##Suffix);                \
