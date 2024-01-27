#pragma once

#include "Common/Precompile.h"

#include "Panel.h"
#include "ResizablePanel.h"
#include "ScrollView.h"

#define _D14_UIKIT_WATERFALL_VIEW_IMPL(Type_Name)                                       \
                                                                                        \
Type_Name::Type_Name() : Type_Name(Passkey{})                                           \
{                                                                                       \
    Panel::pimpl->uiobj =                                                               \
    ResizablePanel::pimpl->uiobj =                                                      \
    ScrollView::pimpl->uiobj =                                                          \
    Type_Name::pimpl->uiobj =                                                           \
    uikit::makeUIObject<uikit::Type_Name>();                                            \
                                                                                        \
    Panel::initialize();                                                                \
    ResizablePanel::initialize();                                                       \
    ScrollView::initialize();                                                           \
    Type_Name::initialize();                                                            \
}                                                                                       \
                                                                                        \
Type_Name::Type_Name(Passkey)                                                           \
    :                                                                                   \
    Panel(Panel::Passkey{}),                                                            \
    ScrollView(ScrollView::Passkey{}),                                                  \
    pimpl(std::make_shared<Impl>()),                                                    \
    pcallback(std::make_unique<Callback>()) { }                                         \
                                                                                        \
void Type_Name::initialize()                                                            \
{                                                                                       \
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
int Type_Name::itemCount() const                                                        \
{                                                                                       \
    return (int)pimpl->uiobj->childrenItems().size();                                   \
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
                                                                                        \
Type_Name::Callback& Type_Name::callback() const                                        \
{                                                                                       \
    return *pcallback;                                                                  \
}                                                                                       \
                                                                                        \
void Type_Name::onSelectChange(const std::set<int>& selected) { }                       \
