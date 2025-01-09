#pragma once

#include "Common.h"

namespace d14uikit { class Label; }

#define _D14_UIKIT_SLIDER_DECL(Type_Name)                                               \
                                                                                        \
Type_Name();                                                                            \
                                                                                        \
float value() const;                                                                    \
void setValue(float value);                                                             \
                                                                                        \
float minValue() const;                                                                 \
void setMinValue(float value);                                                          \
                                                                                        \
float maxValue() const;                                                                 \
void setMaxValue(float value);                                                          \
                                                                                        \
enum class StepMode                                                                     \
{                                                                                       \
    Continuous, Discrete                                                                \
};                                                                                      \
constexpr static auto Continuous = StepMode::Continuous;                                \
constexpr static auto Discrete = StepMode::Discrete;                                    \
                                                                                        \
StepMode stepMode() const;                                                              \
void setStepMode(StepMode value);                                                       \
                                                                                        \
float stepInterval() const;                                                             \
void setStepInterval(float value);                                                      \
                                                                                        \
Label* valueLabel() const;                                                              \
                                                                                        \
bool vlabelEnabled() const;                                                             \
void setVlabelEnabled(bool value);                                                      \
                                                                                        \
int vlabelPrecision() const;                                                            \
void setVlabelPrecision(int value);                                                     \
                                                                                        \
bool vlabelResident() const;                                                            \
void setVlabelResident(bool value);                                                     \
                                                                                        \
int vlabelOffset() const;                                                               \
void setVlabelOffset(int value);                                                        \
                                                                                        \
int vlabelRoundRaidus() const;                                                          \
void setVlabelRoundRaidus(int value);                                                   \
                                                                                        \
Size vlabelRectSize() const;                                                            \
void setVlabelRectSize(const Size& value);                                              \
                                                                                        \
Size vlabelTipSize() const;                                                             \
void setVlabelTipSize(const Size& value);                                               \
                                                                                        \
struct Callback                                                                         \
{                                                                                       \
    std::function<void(Type_Name*, float)> onValueChange = {};                          \
                                                                                        \
    std::function<void(Type_Name*, float)> onEndSliding = {};                           \
                                                                                        \
    std::function<void(Type_Name*, float)> onStartSliding = {};                         \
};                                                                                      \
Callback& callback() const;                                                             \
                                                                                        \
protected:                                                                              \
                                                                                        \
std::unique_ptr<Callback> pcallback = {};                                               \
                                                                                        \
virtual void onValueChange(float value);                                                \
                                                                                        \
virtual void onEndSliding(float value);                                                 \
                                                                                        \
virtual void onStartSliding(float value);                                               \
