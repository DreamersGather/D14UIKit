#pragma once

#define _D14_UIKIT_LAYOUT_DECL(Type_Name)                                               \
                                                                                        \
Type_Name();                                                                            \
                                                                                        \
float opacity() const;                                                                  \
void setOpacity(float value);                                                           \
                                                                                        \
int outlineWidth() const;                                                               \
void setOutlineWidth(int value);                                                        \
                                                                                        \
float outlineOpacity() const;                                                           \
void setOutlineOpacity(float value);                                                    \
                                                                                        \
void addElement(Panel* elem, const GeoInfo& geoInfo);                                   \
                                                                                        \
void removeElement(Panel* elem);                                                        \
void clearAllElements();                                                                \
                                                                                        \
void updateElement(Panel* elem, const GeoInfo& geoInfo);                                \
