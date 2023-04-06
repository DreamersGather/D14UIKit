#pragma once

#include "Common/Precompile.h"

#include "Inc/BasicType.h"

#include "Common/MathUtils/Basic.h"

#define _D14_UIKIT_SLIDER_IMPL(Type_Name)                                               \
                                                                                        \
float Type_Name::value() const                                                          \
{                                                                                       \
    return pimpl->uiobj->value();                                                       \
}                                                                                       \
                                                                                        \
void Type_Name::setValue(float value)                                                   \
{                                                                                       \
    pimpl->uiobj->setValue(value);                                                      \
}                                                                                       \
                                                                                        \
float Type_Name::minValue() const                                                       \
{                                                                                       \
    return pimpl->uiobj->minValue();                                                    \
}                                                                                       \
                                                                                        \
void Type_Name::setMinValue(float value)                                                \
{                                                                                       \
    pimpl->uiobj->setMinValue(value);                                                   \
}                                                                                       \
                                                                                        \
float Type_Name::maxValue() const                                                       \
{                                                                                       \
    return pimpl->uiobj->maxValue();                                                    \
}                                                                                       \
                                                                                        \
void Type_Name::setMaxValue(float value)                                                \
{                                                                                       \
    pimpl->uiobj->setMaxValue(value);                                                   \
}                                                                                       \
                                                                                        \
Type_Name::StepMode Type_Name::stepMode() const                                         \
{                                                                                       \
    return (StepMode)pimpl->uiobj->stepMode;                                            \
}                                                                                       \
                                                                                        \
void Type_Name::setStepMode(StepMode value)                                             \
{                                                                                       \
    pimpl->uiobj->stepMode = (uikit::Slider::StepMode)value;                            \
}                                                                                       \
                                                                                        \
float Type_Name::stepInterval() const                                                   \
{                                                                                       \
    return pimpl->uiobj->stepInterval;                                                  \
}                                                                                       \
                                                                                        \
void Type_Name::setStepInterval(float value)                                            \
{                                                                                       \
    pimpl->uiobj->stepInterval = value;                                                 \
}                                                                                       \
                                                                                        \
Label* Type_Name::valueLabel() const                                                    \
{                                                                                       \
    return pimpl->valueLabel.get();                                                     \
}                                                                                       \
                                                                                        \
bool Type_Name::vlabelEnabled() const                                                   \
{                                                                                       \
    return pimpl->uiobj->getAppearance().valueLabel.enabled;                            \
}                                                                                       \
                                                                                        \
void Type_Name::setVlabelEnabled(bool value)                                            \
{                                                                                       \
    pimpl->uiobj->getAppearance().valueLabel.enabled = value;                           \
}                                                                                       \
                                                                                        \
int Type_Name::vlabelPrecision() const                                                  \
{                                                                                       \
    return pimpl->uiobj->getAppearance().valueLabel.precision;                          \
}                                                                                       \
                                                                                        \
void Type_Name::setVlabelPrecision(int value)                                           \
{                                                                                       \
    pimpl->uiobj->getAppearance().valueLabel.precision = value;                         \
}                                                                                       \
                                                                                        \
bool Type_Name::vlabelResident() const                                                  \
{                                                                                       \
    return pimpl->uiobj->getAppearance().valueLabel.isResident;                         \
}                                                                                       \
                                                                                        \
void Type_Name::setVlabelResident(bool value)                                           \
{                                                                                       \
    pimpl->uiobj->getAppearance().valueLabel.isResident = value;                        \
}                                                                                       \
                                                                                        \
int Type_Name::vlabelOffset() const                                                     \
{                                                                                       \
    return math_utils::round(pimpl->uiobj->getAppearance().valueLabel.offset);          \
}                                                                                       \
                                                                                        \
void Type_Name::setVlabelOffset(int value)                                              \
{                                                                                       \
    pimpl->uiobj->getAppearance().valueLabel.offset = (float)value;                     \
}                                                                                       \
                                                                                        \
int Type_Name::vlabelRoundRaidus() const                                                \
{                                                                                       \
    auto& appear = pimpl->uiobj->getAppearance().valueLabel;                            \
    return math_utils::round(appear.mainRect.geometry.roundRadius);                     \
}                                                                                       \
                                                                                        \
void Type_Name::setVlabelRoundRaidus(int value)                                         \
{                                                                                       \
    auto& appear = pimpl->uiobj->getAppearance().valueLabel;                            \
    appear.mainRect.geometry.roundRadius = (float)value;                                \
}                                                                                       \
                                                                                        \
Size Type_Name::vlabelRectSize() const                                                  \
{                                                                                       \
    auto& appear = pimpl->uiobj->getAppearance().valueLabel;                            \
    auto& rectSize = appear.mainRect.geometry.size;                                     \
    return                                                                              \
    {                                                                                   \
        math_utils::round(rectSize.width),                                              \
        math_utils::round(rectSize.height)                                              \
    };                                                                                  \
}                                                                                       \
                                                                                        \
void Type_Name::setVlabelRectSize(const Size& value)                                    \
{                                                                                       \
    auto& appear = pimpl->uiobj->getAppearance().valueLabel;                            \
    appear.mainRect.geometry.size =                                                     \
    {                                                                                   \
        (float)value.width, (float)value.height                                         \
    };                                                                                  \
    pimpl->uiobj->loadValueLabelShadowBitmap();                                         \
}                                                                                       \
                                                                                        \
Size Type_Name::vlabelTipSize() const                                                   \
{                                                                                       \
    auto& appear = pimpl->uiobj->getAppearance().valueLabel;                            \
    auto& tipSize = appear.sideTriangle.size;                                           \
    return                                                                              \
    {                                                                                   \
        math_utils::round(tipSize.width),                                               \
        math_utils::round(tipSize.height)                                               \
    };                                                                                  \
}                                                                                       \
                                                                                        \
void Type_Name::setVlabelTipSize(const Size& value)                                     \
{                                                                                       \
    auto& appear = pimpl->uiobj->getAppearance().valueLabel;                            \
    appear.sideTriangle.size =                                                          \
    {                                                                                   \
        (float)value.width, (float)value.height                                         \
    };                                                                                  \
    pimpl->uiobj->loadValueLabelShadowBitmap();                                         \
}                                                                                       \
