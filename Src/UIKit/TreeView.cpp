#include "Common/Precompile.h"

#include "UIKit/TreeView.h"

namespace d14engine::uikit
{
    TreeView::TreeView(const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        WaterfallView(rect) { }

    void TreeView::onInitializeFinish()
    {
        WaterfallView::onInitializeFinish();

        m_layout->f_onReleaseUIObject = [this](Panel* p, ShrdPtrParam<Panel> uiobj)
        {
            auto itemobj = std::static_pointer_cast<TreeViewItem>(uiobj);
            if (itemobj)
            {
                size_t index = 0;
                if (itemobj->parentItem().expired())
                {
                    for (auto& item : m_rootItems)
                    {
                        if (cpp_lang_utils::isMostDerivedEqual(uiobj, item))
                        {
                            removeRootItem(index);
                            return true;
                        }
                        ++index;
                    }
                }
                else // managed by another item
                {
                    auto itemobjParent = itemobj->parentItem().lock();
                    for (auto& item : itemobjParent->childrenItems())
                    {
                        if (cpp_lang_utils::isMostDerivedEqual(uiobj, item.ptr))
                        {
                            itemobjParent->removeItem(index);
                            return true;
                        }
                        ++index;
                    }
                }
            }
            return false;
        };
    }

    const TreeView::ItemList& TreeView::rootItems() const
    {
        return m_rootItems;
    }

    void TreeView::insertRootItem(const ItemList& rootitems, size_t rootIndex)
    {
        // "index == m_rootItems.size()" ---> append.
        rootIndex = std::clamp(rootIndex, 0ull, m_rootItems.size());

        auto insertIndex = getRootItemGlobalIndex(rootIndex);

        insertItem(getExpandedTreeViewItems(rootitems), insertIndex.index);

        for (auto& rootItem : rootitems)
        {
            rootItem->m_parentView = std::dynamic_pointer_cast<TreeView>(shared_from_this());
            rootItem->m_nodeLevel = 0;
            rootItem->m_parentItem.reset();
            rootItem->updateSelfContentHorzIndent();
            rootItem->updateChildrenMiscellaneousFields();
        }
        m_rootItems.insert(std::next(m_rootItems.begin(), rootIndex), rootitems.begin(), rootitems.end());
    }

    void TreeView::appendRootItem(const ItemList& rootitems)
    {
        insertRootItem(rootitems, m_rootItems.size());
    }

    void TreeView::removeRootItem(size_t rootIndex, size_t count)
    {
        if (rootIndex >= 0 && rootIndex < m_rootItems.size() && count > 0)
        {
            count = std::min(count, m_rootItems.size() - rootIndex);

            auto removeStartIndex = getRootItemGlobalIndex(rootIndex);
            auto removeLastIndex = getRootItemGlobalIndex(count - 1, removeStartIndex);

            size_t removeCount = removeLastIndex.index - removeStartIndex.index + 1;
            removeItem(removeStartIndex.index, removeCount + (*removeLastIndex)->getExpandedChildrenCount());

            auto baseItor = std::next(m_rootItems.begin(), rootIndex);
            for (size_t i = 0; i < count; ++i)
            {
                (*baseItor)->m_parentView.reset();
                (*baseItor)->m_nodeLevel = 0;
                (*baseItor)->m_parentItem.reset();
                (*baseItor)->updateSelfContentHorzIndent();
                (*baseItor)->updateChildrenMiscellaneousFields();

                baseItor = m_rootItems.erase(baseItor);
            }
        }
    }

    void TreeView::clearAllItems()
    {
        for (auto& item : m_rootItems)
        {
            item->m_parentView.reset();
            item->m_nodeLevel = 0;
            item->m_parentItem.reset();
            item->updateSelfContentHorzIndent();
            item->updateChildrenMiscellaneousFields();
        }
        WaterfallView::clearAllItems();
    }

    float TreeView::baseHorzIndent() const
    {
        return m_baseHorzIndent;
    }

    void TreeView::setBaseHorzIndent(float value)
    {
        m_baseHorzIndent = value;
        for (auto& item : m_items) item->updateContentHorzIndent();
    }

    float TreeView::horzIndentEachNodelLevel() const
    {
        return m_horzIndentEachNodeLevel;
    }

    void TreeView::setHorzIndentEachNodelLevel(float value)
    {
        m_horzIndentEachNodeLevel = value;
        for (auto& item : m_items) item->updateContentHorzIndent();
    }

    void TreeView::setItemIndexRangeActive(bool value)
    {
        auto& range = m_activeItemIndexRange;

        if (range.first.valid() && range.last.valid())
        {
            if (value)
            {
                for (ItemIndex itemIndex = range.first; itemIndex <= range.last; ++itemIndex)
                {
                    auto value = (*itemIndex)->m_currState.ancestorUnfolded();

                    // When folding a tree view item, we set its height to
                    // zero to hide the content, but its rect, however, is
                    // still within the scope of the visible area, so we must
                    // make sure the item is not belonging to any folded item
                    // before showing it.

                    (*itemIndex)->setPrivateVisible(value);
                    (*itemIndex)->appEventReactability.hitTest = value;
                }
            }
            else // It is always safe to deactivate an item.
            {
                for (ItemIndex itemIndex = range.first; itemIndex <= range.last; ++itemIndex)
                {
                    (*itemIndex)->setPrivateVisible(false);
                    (*itemIndex)->appEventReactability.hitTest = false;
                }
            }
        }
    }

    TreeView::ItemIndex TreeView::getRootItemGlobalIndex(size_t rootIndex, Optional<ItemIndex> start) const
    {
        if (!m_items.empty())
        {
            if (!start.has_value()) start = ItemIndex::begin((ItemList*)&m_items);
            for (size_t i = 0; i < rootIndex; ++i)
            {
                start.value().moveNext((*start.value())->getExpandedChildrenCount() + 1);
                if (start.value() >= m_items.size()) return ItemIndex::end((ItemList*)&m_items);
            }
#pragma warning(push)
#pragma warning(disable : 26816)
            // Warning C26816: The pointer points to memory allocated on the stack (ES.65)
            // The result returned here is a copy. Not sure why MSVC gives this warning.
            return start.value();
#pragma warning(pop)
        }
        else return ItemIndex::end((ItemList*)&m_items);
    }
}
