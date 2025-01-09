#pragma once

#include "Common/Precompile.h"

#include "Common.h"

// Raw Implementation

#define _D14_UIKIT_WATERFALL_VIEW_RAWI(Type_Name, Item_Type_Name)                       \
                                                                                        \
int Type_Name::itemCount() const                                                        \
{                                                                                       \
    return (int)pimpl->uiobj->childrenItems().size();                                   \
}                                                                                       \
                                                                                        \
/* The return value may be null */                                                      \
Item_Type_Name* Type_Name::getItem(int index) const                                     \
{                                                                                       \
    auto& items = pimpl->uiobj->childrenItems();                                        \
    if (0 <= index && index < (int)items.size())                                        \
    {                                                                                   \
        auto item = std::next(items.begin(), index);                                    \
        pimpl->item = std::shared_ptr<Item_Type_Name>(new Item_Type_Name(*item));       \
    }                                                                                   \
    else pimpl->item.reset();                                                           \
                                                                                        \
    return pimpl->item.get();                                                           \
}                                                                                       \
                                                                                        \
std::set<int> Type_Name::selectedIndicies() const                                       \
{                                                                                       \
    using ViewType = uikit::Type_Name::WaterfallView;                                   \
    using ItemType = ViewType::ItemIndexSet::value_type;                                \
                                                                                        \
    auto& selected = pimpl->uiobj->selectedItemIndices();                               \
                                                                                        \
    std::set<int> indices = {};                                                         \
    std::transform(                                                                     \
        selected.begin(), selected.end(),                                               \
        std::inserter(indices, indices.begin()),                                        \
        [](const ItemType& s) { return (int)s.index; });                                \
                                                                                        \
    return indices;                                                                     \
}                                                                                       \
                                                                                        \
Type_Name::SelectMode Type_Name::selectMode() const                                     \
{                                                                                       \
    return (SelectMode)pimpl->uiobj->selectMode;                                        \
}                                                                                       \
                                                                                        \
void Type_Name::setSelectMode(SelectMode mode)                                          \
{                                                                                       \
    using ViewType = uikit::Type_Name::WaterfallView;                                   \
                                                                                        \
    pimpl->uiobj->selectMode = (ViewType::SelectMode)mode;                              \
}                                                                                       \

// Full Implementation

#define _D14_UIKIT_WATERFALL_VIEW_IMPL(Type_Name, Item_Type_Name)                       \
        _D14_UIKIT_WATERFALL_VIEW_RAWI(Type_Name, Item_Type_Name)                       \
                                                                                        \
Type_Name::Type_Name()                                                                  \
    :                                                                                   \
    Type_Name(uikit::makeUIObject<uikit::Type_Name>()) { }                              \
                                                                                        \
_D14_UIKIT_CTOR(Type_Name)                                                              \
    :                                                                                   \
    Panel(uiobj),                                                                       \
    ScrollView(uiobj),                                                                  \
    pimpl(std::make_shared<Impl>()),                                                    \
    pcallback(std::make_unique<Callback>())                                             \
{                                                                                       \
    pimpl->uiobj = uiobj;                                                               \
                                                                                        \
    using ViewType = uikit::Type_Name::WaterfallView;                                   \
    using ItemType = ViewType::ItemIndexSet::value_type;                                \
                                                                                        \
    pimpl->uiobj->f_onSelectChange = [this]                                             \
    (ViewType* v, const ViewType::ItemIndexSet& selected)                               \
    {                                                                                   \
        std::set<int> indices = {};                                                     \
        std::transform(                                                                 \
            selected.begin(), selected.end(),                                           \
            std::inserter(indices, indices.begin()),                                    \
            [](const ItemType& s) { return (int)s.index; });                            \
                                                                                        \
        onSelectChange(indices);                                                        \
        if (pcallback->onSelectChange)                                                  \
        {                                                                               \
            pcallback->onSelectChange(this, indices);                                   \
        }                                                                               \
    };                                                                                  \
}                                                                                       \
                                                                                        \
Type_Name::Callback& Type_Name::callback() const { return *pcallback; }                 \
                                                                                        \
void Type_Name::onSelectChange(const std::set<int>& selected) { }                       \
