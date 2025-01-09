#pragma once

#include "Common/Precompile.h"

#include "Common.h"

#include "Common/MathUtils/Basic.h"

#define _D14_UIKIT_LAYOUT_IMPL(Type_Name)                                               \
                                                                                        \
Type_Name::Type_Name()                                                                  \
    :                                                                                   \
    Type_Name(uikit::makeUIObject<uikit::Type_Name>()) { }                              \
                                                                                        \
_D14_UIKIT_CTOR(Type_Name)                                                              \
    :                                                                                   \
    Panel(uiobj),                                                                       \
    ResizablePanel(uiobj),                                                              \
    pimpl(std::make_shared<Impl>())                                                     \
{                                                                                       \
    pimpl->uiobj = uiobj;                                                               \
}                                                                                       \
                                                                                        \
float Type_Name::opacity() const                                                        \
{                                                                                       \
    return pimpl->uiobj->getAppearance().background.opacity;                            \
}                                                                                       \
                                                                                        \
void Type_Name::setOpacity(float value)                                                 \
{                                                                                       \
    pimpl->uiobj->getAppearance().background.opacity = value;                           \
}                                                                                       \
                                                                                        \
int Type_Name::outlineWidth() const                                                     \
{                                                                                       \
    return math_utils::round(pimpl->uiobj->getAppearance().stroke.width);               \
}                                                                                       \
                                                                                        \
void Type_Name::setOutlineWidth(int value)                                              \
{                                                                                       \
    pimpl->uiobj->getAppearance().stroke.width = (float)value;                          \
}                                                                                       \
                                                                                        \
float Type_Name::outlineOpacity() const                                                 \
{                                                                                       \
    return pimpl->uiobj->getAppearance().stroke.opacity;                                \
}                                                                                       \
                                                                                        \
void Type_Name::setOutlineOpacity(float value)                                          \
{                                                                                       \
    pimpl->uiobj->getAppearance().stroke.opacity = value;                               \
}                                                                                       \
