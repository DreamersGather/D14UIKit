#pragma once

#include "Common/Precompile.h"

#include "Common.h"

#define _D14_CALLBACK_PROPERTY(Class_Name, Callback_Name) \
    i.def_property( \
        "f_" #Callback_Name, \
        [](const Class_Name& i) \
        { \
            return i.callback().Callback_Name; \
        }, \
        [](const Class_Name& i, \
           const decltype(i.callback().Callback_Name)& f) \
        { \
            i.callback().Callback_Name = f; \
        })
