#pragma once

#include "Common.h"

// Raw Declaration

#define _D14_UIKIT_WATERFALL_VIEW_RAWD(Type_Name, Item_Type_Name)                       \
                                                                                        \
int itemCount() const;                                                                  \
/* The return value may be null */                                                      \
Item_Type_Name* getItem(int index) const;                                               \
                                                                                        \
std::set<int> selectedIndicies() const;                                                 \
                                                                                        \
enum class SelectMode                                                                   \
{                                                                                       \
    None, Single, Multiple, Extended                                                    \
};                                                                                      \
SelectMode selectMode() const;                                                          \
void setSelectMode(SelectMode mode);                                                    \
                                                                                        \
constexpr static auto None = SelectMode::None;                                          \
constexpr static auto Single = SelectMode::Single;                                      \
constexpr static auto Multiple = SelectMode::Multiple;                                  \
constexpr static auto Extended = SelectMode::Extended;                                  \

// Full Declaration

#define _D14_UIKIT_WATERFALL_VIEW_DECL(Type_Name, Item_Type_Name)                       \
        _D14_UIKIT_WATERFALL_VIEW_RAWD(Type_Name, Item_Type_Name)                       \
                                                                                        \
Type_Name();                                                                            \
                                                                                        \
struct Callback                                                                         \
{                                                                                       \
    std::function<void(Type_Name*, const std::set<int>&)> onSelectChange = {};          \
};                                                                                      \
Callback& callback() const;                                                             \
                                                                                        \
protected:                                                                              \
                                                                                        \
std::unique_ptr<Callback> pcallback = {};                                               \
                                                                                        \
virtual void onSelectChange(const std::set<int>& selected);                             \
