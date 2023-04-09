#pragma once

#include "Common/Precompile.h"

#include "Common.h"
#include "Label.h"
#include "Panel.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/Label.h"

#define _D14_UIKIT_SLIDER_IMPL(Type_Name)                                               \
                                                                                        \
Type_Name::Type_Name() : Type_Name(Passkey{})                                           \
{                                                                                       \
    Panel::pimpl->uiobj =                                                               \
    Type_Name::pimpl->uiobj =                                                           \
    uikit::makeUIObject<uikit::Type_Name>();                                            \
                                                                                        \
    Panel::initialize();                                                                \
    Type_Name::initialize();                                                            \
}                                                                                       \
                                                                                        \
Type_Name::Type_Name(Passkey)                                                           \
    :                                                                                   \
    Panel(Panel::Passkey{}),                                                            \
    pimpl(std::make_shared<Impl>()),                                                    \
    pcallback(std::make_unique<Callback>()) { }                                         \
                                                                                        \
void Type_Name::initialize()                                                            \
{                                                                                       \
    pimpl->uiobj->f_onValueChange = [this]                                              \
    (uikit::Slider::ValuefulObject* obj, float value)                                   \
    {                                                                                   \
        onValueChange(value);                                                           \
        if (pcallback->onValueChange)                                                   \
        {                                                                               \
            pcallback->onValueChange(this, value);                                      \
        }                                                                               \
    };                                                                                  \
    pimpl->uiobj->f_onStartSliding = [this]                                             \
    (uikit::Slider* sldr, float value)                                                  \
    {                                                                                   \
        onStartSliding(value);                                                          \
        if (pcallback->onStartSliding)                                                  \
        {                                                                               \
            pcallback->onStartSliding(this, value);                                     \
        }                                                                               \
    };                                                                                  \
    pimpl->uiobj->f_onEndSliding = [this]                                               \
    (uikit::Slider* sldr, float value)                                                  \
    {                                                                                   \
        onEndSliding(value);                                                            \
        if (pcallback->onEndSliding)                                                    \
        {                                                                               \
            pcallback->onEndSliding(this, value);                                       \
        }                                                                               \
    };                                                                                  \
    /* Bind the existing label implementation to the interface. */                      \
    {                                                                                   \
        pimpl->valueLabel = std::shared_ptr<Label>(new Label(Label::Passkey{}));        \
                                                                                        \
        pimpl->valueLabel->Panel::pimpl->uiobj =                                        \
        pimpl->valueLabel->Label::pimpl->uiobj =                                        \
        pimpl->uiobj->valueLabel();                                                     \
                                                                                        \
        pimpl->valueLabel->Panel::initialize();                                         \
        pimpl->valueLabel->Label::initialize();                                         \
    }                                                                                   \
}                                                                                       \
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
    return convert(appear.mainRect.geometry.size);                                      \
}                                                                                       \
                                                                                        \
void Type_Name::setVlabelRectSize(const Size& value)                                    \
{                                                                                       \
    auto& appear = pimpl->uiobj->getAppearance().valueLabel;                            \
    appear.mainRect.geometry.size = convert(value);                                     \
    pimpl->uiobj->loadValueLabelShadowBitmap();                                         \
}                                                                                       \
                                                                                        \
Size Type_Name::vlabelTipSize() const                                                   \
{                                                                                       \
    auto& appear = pimpl->uiobj->getAppearance().valueLabel;                            \
    return convert(appear.sideTriangle.size);                                           \
}                                                                                       \
                                                                                        \
void Type_Name::setVlabelTipSize(const Size& value)                                     \
{                                                                                       \
    auto& appear = pimpl->uiobj->getAppearance().valueLabel;                            \
    appear.sideTriangle.size = convert(value);                                          \
    pimpl->uiobj->loadValueLabelShadowBitmap();                                         \
}                                                                                       \
                                                                                        \
Type_Name::Callback& Type_Name::callback() const { return *pcallback; }                 \
                                                                                        \
void Type_Name::onValueChange(float value) { }                                          \
                                                                                        \
void Type_Name::onEndSliding(float value) { }                                           \
                                                                                        \
void Type_Name::onStartSliding(float value) { }                                         \
                                                                                        \
