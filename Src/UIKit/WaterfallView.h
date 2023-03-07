#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/IndexIterator.h"
#include "Common/CppLangUtils/PointerEquality.h"
#include "UIKit/ConstraintLayout.h"
#include "UIKit/ScrollView.h"
#include "UIKit/ViewItem.h"

namespace d14engine::uikit
{
    template<typename Item_T>
    struct WaterfallView : ScrollView
    {
        static_assert(std::is_base_of_v<ViewItem, Item_T>,
            "Item_T must inherit from d14engine::uikit::ViewItem");

        explicit WaterfallView(const D2D1_RECT_F& rect = {})
            :
            Panel(rect, resource_utils::g_solidColorBrush),
            ScrollView(makeUIObject<ConstraintLayout>(), rect)
        {
            appEventReactability.focus.get = true;
        }

        void onInitializeFinish() override
        {
            ScrollView::onInitializeFinish();

            m_layout = std::dynamic_pointer_cast<ConstraintLayout>(m_content);

            m_layout->f_destroyUIObject = [this](Panel* p, ShrdPtrParam<Panel> uiobj)
            {
                size_t index = 0;
                for (auto& item : m_items)
                {
                    if (cpp_lang_utils::isMostDerivedEqual(uiobj, item))
                    {
                        removeItem(index);
                        return true;
                    }
                    ++index;
                }
                return false;
            };
            m_layout->f_onSize = [this](Panel* p, SizeEvent& e)
            {
                // The viewport offset may be out of range after resizing.
                setViewportOffsetDirect(m_viewportOffset);
                onViewportOffsetChange(m_viewportOffset);
            };
            m_layout->f_onParentSize = [](Panel* p, SizeEvent& e)
            {
                p->resize(e.size.width, p->height());
            };
        }

    public:
        using ItemList = std::list<SharedPtr<Item_T>>;

        using ItemIndex = cpp_lang_utils::IndexIterator<ItemList>;

        using ItemIndexSet = std::set<ItemIndex, std::less</* Heterogeneous Lookup */>>;

        void onSelectChange(const ItemIndexSet& selected)
        {
            onSelectChangeHelper(selected);

            if (f_onSelectChange) f_onSelectChange(this, selected);
        }
        Function<void(WaterfallView*, const ItemIndexSet&)> f_onSelectChange = {};

    protected:
        virtual void onSelectChangeHelper(const ItemIndexSet& selected) { }

    protected:
        ItemList m_items = {};

        ItemIndexSet m_selectedItemIndices = {};

    public:
        const ItemList& childrenItems() const { return m_items; }

        const ItemIndexSet& selectedItemIndices() const { return m_selectedItemIndices; }

        virtual void insertItem(const ItemList& items, size_t index = 0)
        {
            // "index == m_items.size()" ---> append
            index = std::clamp(index, 0ull, m_items.size());
            
            float height = 0.0f;
            for (auto& item : items)
            {
                height += item->height();
            }
            m_layout->resize(width(), m_layout->height() + height);

            auto itemIndex = ItemIndex::begin(&m_items);

            float offset = 0.0f;
            // Lower Items
            for ( ; itemIndex < index; ++itemIndex )
            {
                offset += (*itemIndex)->height();
            }
            ItemIndex insertIndex = itemIndex;
            // New Items
            for (auto& item : items)
            {
                item->setVisible(false);
                item->setEnabled(false);

                ConstraintLayout::GeometryInfo info = {};

                info.keepWidth = false;
                info.Left.ToLeft = 0.0f;
                info.Right.ToRight = 0.0f;
                info.Top.ToTop = offset;

                m_layout->addElement(item, info);

                offset += item->height();
            }
            // Higher Items
            for ( ; itemIndex < m_items.size(); ++itemIndex )
            {
                auto elemItor = m_layout->findElement(*itemIndex);
                if (elemItor.has_value())
                {
                    elemItor.value()->second.Top.ToTop = offset;
                    m_layout->updateElement(elemItor.value());
                }
                offset += (*itemIndex)->height();
            }
            m_items.insert(insertIndex.iterator, items.begin(), items.end());

            ItemIndexSet updatedItemIndexSet = {};
            for (auto& itemIndex : m_selectedItemIndices)
            {
                if (itemIndex < index)
                {
                    updatedItemIndexSet.insert(itemIndex);
                }
                else updatedItemIndexSet.insert(itemIndex.getIndexNext(items.size()));
            }
            m_selectedItemIndices = std::move(updatedItemIndexSet);

#define UPDATE_ITEM_INDEX(Item_Index) \
do { \
    if (Item_Index.generalValid()) \
    { \
        if (Item_Index >= index) \
        { \
            Item_Index.moveIndexNext(items.size()); \
        } \
    } \
} while (0)
            UPDATE_ITEM_INDEX(m_extendedSelectItemIndexOrigin);
            UPDATE_ITEM_INDEX(m_lastSelectedItemIndex);
            UPDATE_ITEM_INDEX(m_lastHoverItemIndex);

            UPDATE_ITEM_INDEX(m_activeItemIndexRange.first);
            UPDATE_ITEM_INDEX(m_activeItemIndexRange.last);

#undef UPDATE_ITEM_INDEX

            updateItemIndexRangeActivity();
        }

        virtual void removeItem(size_t index, size_t count = 1)
        {
            if (index >= 0 && index < m_items.size() && count > 0)
            {
                count = std::min(count, m_items.size() - index);
                size_t endIndex = index + count;

                float height = 0.0f;
                for (ItemIndex itemIndex = { &m_items, index }; itemIndex < endIndex; ++itemIndex)
                {
                    height += (*itemIndex)->height();
                }
                m_layout->resize(width(), m_layout->height() - height);

                auto itemIndex = ItemIndex::begin(&m_items);

                float offset = 0.0f;
                // Lower Items
                for (; itemIndex < index; ++itemIndex)
                {
                    offset += (*itemIndex)->height();
                }
                ItemIndex eraseStartIndex = itemIndex;
                // Old Items
                for (; itemIndex < endIndex; ++itemIndex)
                {
                    m_layout->removeElement(*itemIndex);
                }
                ItemIndex eraseEndIndex = itemIndex;
                // Higher Items
                for (; itemIndex < m_items.size(); ++itemIndex)
                {
                    auto elemItor = m_layout->findElement(*itemIndex);
                    if (elemItor.has_value())
                    {
                        elemItor.value()->second.Top.ToTop = offset;
                        m_layout->updateElement(elemItor.value());
                    }
                    offset += (*itemIndex)->height();
                }
                m_items.erase(eraseStartIndex.iterator, eraseEndIndex.iterator);

                ItemIndexSet updatedItemIndexSet = {};
                for (auto& itemIndex : m_selectedItemIndices)
                {
                    if (itemIndex < index)
                    {
                        updatedItemIndexSet.insert(itemIndex);
                    }
                    else if (itemIndex >= endIndex)
                    {
                        updatedItemIndexSet.insert(itemIndex.getIndexPrev(count));
                    }
                }
                m_selectedItemIndices = std::move(updatedItemIndexSet);

#define UPDATE_ITEM_INDEX(Item_Index) \
do { \
    if (Item_Index.generalValid()) \
    { \
        if (Item_Index >= endIndex) \
        { \
            Item_Index.moveIndexPrev(count); \
        } \
        else if (Item_Index >= index) \
        { \
            Item_Index.invalidate(); \
        } \
    } \
} while (0)
                UPDATE_ITEM_INDEX(m_lastHoverItemIndex);
                UPDATE_ITEM_INDEX(m_lastSelectedItemIndex);
                UPDATE_ITEM_INDEX(m_extendedSelectItemIndexOrigin);

#undef UPDATE_ITEM_INDEX

                if (m_activeItemIndexRange.first.generalValid() &&
                    m_activeItemIndexRange.last.generalValid())
                {
                    if (m_activeItemIndexRange.first >= endIndex)
                    {
                        m_activeItemIndexRange.first.moveIndexPrev(count);
                        m_activeItemIndexRange.last.moveIndexPrev(count);
                    }
                    else if (m_activeItemIndexRange.first >= index)
                    {
                        if (m_activeItemIndexRange.last >= endIndex)
                        {
                            m_activeItemIndexRange.first = eraseEndIndex.getIndexPrev(count);
                        }
                        else // all visible items removed
                        {
                            m_activeItemIndexRange.first.invalidate();
                            m_activeItemIndexRange.last.invalidate();
                        }
                    }
                    else if (m_activeItemIndexRange.last >= endIndex)
                    {
                        m_activeItemIndexRange.last.moveIndexPrev(count);
                    }
                    else if (m_activeItemIndexRange.last >= index)
                    {
                        m_activeItemIndexRange.last = eraseEndIndex.getIndexPrev(count + 1);
                    }
                }
                updateItemIndexRangeActivity();
            }
        }

        virtual void clearAllItems()
        {
            m_layout->clearAllElements();
            m_layout->resize(m_layout->width(), 0.0f);

            m_items.clear();

            m_extendedSelectItemIndexOrigin.invalidate();
            m_lastSelectedItemIndex.invalidate();
            m_lastHoverItemIndex.invalidate();

            m_activeItemIndexRange.first.invalidate();
            m_activeItemIndexRange.last.invalidate();
        }

    protected:
        SharedPtr<ConstraintLayout> m_layout = {};

    public:
        void updateItemConstraints()
        {
            float offset = 0.0f;
            for (auto& item : m_items)
            {
                auto elemItor = m_layout->findElement(item);
                if (elemItor.has_value())
                {
                    elemItor.value()->second.Top.ToTop = offset;
                    m_layout->updateElement(elemItor.value());
                }
                offset += item->height();
            }
            m_layout->resize(width(), offset);
        }

    protected:
        using ItemIndexParam = const ItemIndex&;

        ItemIndex viewportOffsetToItemIndex(float offset) const
        {
            float itemHeight = 0.0f;
            for (ItemIndex itemIndex = { (ItemList*)&m_items, 0}; itemIndex.valid(); ++itemIndex)
            {
                // Do not use "itemHeight < offset" since it might cause the
                // unexpected capture failure when the cursor-point is right
                // on the edge of an item.
                if (itemHeight <= offset)
                {
                    itemHeight += (*itemIndex)->height();
                    if (itemHeight > offset) return itemIndex;
                }
                else break;
            }
            return ItemIndex{};
        }

    public:
        enum class SelectMode
        {
            None, Single, Multiple, Extended
        }
        selectMode = SelectMode::Extended;

    protected:
        ItemIndex m_lastHoverItemIndex{};
        ItemIndex m_lastSelectedItemIndex{};

        ItemIndex m_extendedSelectItemIndexOrigin{};

        void triggerNoneSelect()
        {
            for (auto& itemIndex : m_selectedItemIndices)
            {
                (*itemIndex)->triggerUnchkStateTrans();
            }
            m_selectedItemIndices.clear();

            m_lastSelectedItemIndex.invalidate();
            m_extendedSelectItemIndexOrigin.invalidate();
        }

        void triggerSingleSelect(ItemIndexParam itemIndex)
        {
            for (auto& tmpItemIndex : m_selectedItemIndices)
            {
                (*tmpItemIndex)->triggerUnchkStateTrans();
            }
            (*itemIndex)->triggerCheckStateTrans();

            m_selectedItemIndices = { itemIndex };

            m_lastSelectedItemIndex = m_extendedSelectItemIndexOrigin = itemIndex;
        }

        void triggerMultipleSelect(ItemIndexParam itemIndex)
        {
            auto itemIndexItor = m_selectedItemIndices.find(itemIndex);
            if (itemIndexItor != m_selectedItemIndices.end())
            {
                (*itemIndex)->triggerUnchkStateTrans();

                m_selectedItemIndices.erase(itemIndexItor);

                if (m_lastSelectedItemIndex == itemIndex)
                {
                    m_lastSelectedItemIndex.invalidate();
                }
                if (m_extendedSelectItemIndexOrigin == itemIndex)
                {
                    m_extendedSelectItemIndexOrigin.invalidate();
                }
            }
            else // select new item
            {
                (*itemIndex)->triggerCheckStateTrans();

                m_selectedItemIndices.insert(itemIndex);

                if (m_lastSelectedItemIndex.valid())
                {
                    (*m_lastSelectedItemIndex)->triggerLeaveStateTrans();
                }
                m_lastSelectedItemIndex = m_extendedSelectItemIndexOrigin = itemIndex;
            }
        }

        void triggerExtendedSelect(ItemIndexParam itemIndex)
        {
            if (KeyboardEvent::SHIFT())
            {
                if (!m_selectedItemIndices.empty())
                {
                    for (auto& tmpItemIndex : m_selectedItemIndices)
                    {
                        (*tmpItemIndex)->triggerUnchkStateTrans();
                    }
                    m_selectedItemIndices.clear();

                    auto range = std::minmax(itemIndex, m_extendedSelectItemIndexOrigin);
                    for (auto tmpItemIndex = range.first; tmpItemIndex <= range.second; ++tmpItemIndex)
                    {
                        (*tmpItemIndex)->triggerCheckStateTrans();

                        if (tmpItemIndex != itemIndex) // Highlight only the last selected item.
                        {
                            (*tmpItemIndex)->triggerLeaveStateTrans();
                        }
                        m_selectedItemIndices.insert(tmpItemIndex);
                    }
                    m_lastSelectedItemIndex = itemIndex;
                }
                else triggerSingleSelect(itemIndex);
            }
            else if (KeyboardEvent::CTRL())
            {
                triggerMultipleSelect(itemIndex);
            }
            else triggerSingleSelect(itemIndex);
        }

    protected:
        struct VisibleItemIndexRange
        {
            ItemIndex first{}, last{};
        }
        m_activeItemIndexRange = {};

        virtual void setItemIndexRangeActive(bool value)
        {
            auto& range = m_activeItemIndexRange;

            if (range.first.valid() && range.last.valid())
            {
                for (auto itemIndex = range.first; itemIndex <= range.last; ++itemIndex)
                {
                    (*itemIndex)->setVisible(value);
                    (*itemIndex)->setEnabled(value);
                }
            }
        }

    public:
        void updateItemIndexRangeActivity()
        {
            setItemIndexRangeActive(false);

            m_activeItemIndexRange.first = viewportOffsetToItemIndex
            (
                m_viewportOffset.y
            );
            m_activeItemIndexRange.last  = viewportOffsetToItemIndex
            (
                m_viewportOffset.y + height()
            );
            if (m_activeItemIndexRange.first.valid() && !m_activeItemIndexRange.last.valid())
            {
                m_activeItemIndexRange.last = ItemIndex::last(&m_items);
            }
            setItemIndexRangeActive(true);
        }

    protected:
        // Panel
        void onGetFocusHelper() override
        {
            ScrollView::onGetFocusHelper();

            for (auto& itemIndex : m_selectedItemIndices)
            {
                (*itemIndex)->triggerGetfcStateTrans();
            }
        }

        void onLoseFocusHelper() override
        {
            ScrollView::onLoseFocusHelper();

            for (auto& itemIndex : m_selectedItemIndices)
            {
                (*itemIndex)->triggerLosfcStateTrans();
            }
        }

        void onMouseMoveHelper(MouseMoveEvent& e) override
        {
            ScrollView::onMouseMoveHelper(e);

            auto& p = e.cursorPoint;
            auto relative = absoluteToSelfCoord(p);

            // In case trigger by mistake when controlling the scroll bars.
            auto itemIndex = isControllingScrollBars() ? ItemIndex{} :
                viewportOffsetToItemIndex(m_viewportOffset.y + relative.y);

            if (itemIndex.valid() && itemIndex != m_lastHoverItemIndex)
            {
                (*itemIndex)->triggerEnterStateTrans();
            }
            if (m_lastHoverItemIndex.valid() && m_lastHoverItemIndex != itemIndex)
            {
                (*m_lastHoverItemIndex)->triggerLeaveStateTrans();
            }
            m_lastHoverItemIndex = itemIndex;
        }

        void onMouseLeaveHelper(MouseMoveEvent& e) override
        {
            ScrollView::onMouseLeaveHelper(e);

            if (m_lastHoverItemIndex.valid())
            {
                (*m_lastHoverItemIndex)->triggerLeaveStateTrans();
            }
            m_lastHoverItemIndex.invalidate();
        }

        void onMouseButtonHelper(MouseButtonEvent& e) override
        {
            ScrollView::onMouseButtonHelper(e);

            auto& p = e.cursorPoint;
            auto relative = absoluteToSelfCoord(p);

            if (e.state.leftDown())
            {
                // In case trigger by mistake when controlling the scroll bars.
                auto itemIndex = isControllingScrollBars() ? ItemIndex{} :
                    viewportOffsetToItemIndex(m_viewportOffset.y + relative.y);

                if (itemIndex.valid() && (*itemIndex)->m_enabled)
                {
                    switch (selectMode)
                    {
                    case SelectMode::None: triggerNoneSelect(); break;
                    case SelectMode::Single: triggerSingleSelect(itemIndex); break;
                    case SelectMode::Multiple: triggerMultipleSelect(itemIndex); break;
                    case SelectMode::Extended: triggerExtendedSelect(itemIndex); break;
                    default: break;
                    }
                    onSelectChange(m_selectedItemIndices);
                }
            }
        }

        // ScrollView
        void onViewportOffsetChangeHelper(const D2D1_POINT_2F& offset) override
        {
            ScrollView::onViewportOffsetChangeHelper(offset);

            updateItemIndexRangeActivity();
        }
    };
}
