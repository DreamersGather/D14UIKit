#pragma once

#include "BasicType.h"

namespace d14uikit { class Label; }

#define _D14_UIKIT_SLIDER_DECL                            \
                                                          \
float value() const;                                      \
void setValue(float value);                               \
                                                          \
float minValue() const;                                   \
void setMinValue(float value);                            \
                                                          \
float maxValue() const;                                   \
void setMaxValue(float value);                            \
                                                          \
enum class StepMode                                       \
{                                                         \
    Continuous, Discrete                                  \
};                                                        \
StepMode stepMode() const;                                \
void setStepMode(StepMode value);                         \
                                                          \
float stepInterval() const;                               \
void setStepInterval(float value);                        \
                                                          \
Label* valueLabel() const;                                \
                                                          \
bool vlabelEnabled() const;                               \
void setVlabelEnabled(bool value);                        \
                                                          \
int vlabelPrecision() const;                              \
void setVlabelPrecision(int value);                       \
                                                          \
bool vlabelResident() const;                              \
void setVlabelResident(bool value);                       \
                                                          \
int vlabelOffset() const;                                 \
void setVlabelOffset(int value);                          \
                                                          \
int vlabelRoundRaidus() const;                            \
void setVlabelRoundRaidus(int value);                     \
                                                          \
Size vlabelRectSize() const;                              \
void setVlabelRectSize(const Size& value);                \
                                                          \
Size vlabelTipSize() const;                               \
void setVlabelTipSize(const Size& value);                 \
