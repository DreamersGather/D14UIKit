#pragma once

#include "Common/Precompile.h"

#include "Panel.h"
#include "ResizablePanel.h"

#include "Common/MathUtils/Basic.h"

#define _D14_UIKIT_LAYOUT_IMPL(Type_Name)                                               \
                                                                                        \
Type_Name::Type_Name() : Type_Name(Passkey{})                                           \
{                                                                                       \
    Panel::pimpl->uiobj =                                                               \
    ResizablePanel::pimpl->uiobj =                                                      \
    Type_Name::pimpl->uiobj =                                                           \
    uikit::makeUIObject<uikit::Type_Name>();                                            \
                                                                                        \
    Panel::initialize();                                                                \
    ResizablePanel::initialize();                                                       \
    Type_Name::initialize();                                                            \
}                                                                                       \
                                                                                        \
Type_Name::Type_Name(Passkey)                                                           \
    :                                                                                   \
    Panel(Panel::Passkey{}),                                                            \
    ResizablePanel(ResizablePanel::Passkey{}),                                          \
    pimpl(std::make_shared<Impl>()) { }                                                 \
                                                                                        \
void Type_Name::initialize() { }                                                        \
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
