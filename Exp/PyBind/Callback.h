#pragma once

#ifdef _D14_UIKIT_EXP_PY_BIND

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

#endif
