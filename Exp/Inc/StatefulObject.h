#pragma once

#include "Common.h"

#define _D14_UIKIT_STATEFUL_OBJECT_DECL(Type_Name)                                      \
                                                                                        \
State state() const;                                                                    \
void setState(State state);                                                             \
                                                                                        \
struct Callback                                                                         \
{                                                                                       \
    std::function<void(Type_Name*, State)> onStateChange = {};                          \
};                                                                                      \
Callback& callback() const;                                                             \
                                                                                        \
protected:                                                                              \
                                                                                        \
std::unique_ptr<Callback> pcallback = {};                                               \
                                                                                        \
virtual void onStateChange(State state);                                                \
