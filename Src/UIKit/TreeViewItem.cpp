#include "Common/Precompile.h"

#include "UIKit/TreeViewItem.h"

#include "Common/CppLangUtils/PointerEquality.h"
#include "Common/MathUtils/2D.h"
#include "Renderer/Renderer.h"
#include "UIKit/IconLabel.h"
#include "UIKit/ResourceUtils.h"
#include "UIKit/TreeView.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    TreeViewItem::TreeViewItem(
        ShrdPtrParam<Panel> content,
        const D2D1_RECT_F& rect)
        :
        ViewItem(content, rect)
    {
        StatefulObject::m_state = { UNFOLDED };
        StatefulObject::m_currState.flag = UNFOLDED;
        StatefulObject::m_currState.ancestorFlag = UNFOLDED;

        m_layout = makeUIObject<ConstraintLayout>();
    }

    TreeViewItem::TreeViewItem(WstrParam text, const D2D1_RECT_F& rect)
        : TreeViewItem(IconLabel::compactLayout(text), rect) { }


    void TreeViewItem::onInitializeFinish()
    {
        ViewItem::onInitializeFinish();

        addUIObject(m_layout);

        m_layout->transform(selfCoordRect());

        if (m_content != nullptr)
        {
            ConstraintLayout::GeometryInfo info = {};

            info.keepWidth = info.keepHeight = false;
            info.Left.ToLeft = info.Right.ToRight = 0.0f;
            info.Top.ToTop = info.Bottom.ToBottom = 0.0f;

            m_layout->addElement(m_content, info);

            removeUIObject(m_content);
        }
        m_content = m_layout;
    }

    WeakPtr<Panel> TreeViewItem::content() const
    {
        if (m_layout->children().empty()) return {};
        else return *m_layout->children().begin();
    }

    void TreeViewItem::setContent(ShrdPtrParam<Panel> content)
    {
        auto originalContent = TreeViewItem::content().lock();
        if (content && !cpp_lang_utils::isMostDerivedEqual(content, originalContent))
        {
            m_layout->removeElement(originalContent);

            ConstraintLayout::GeometryInfo info = {};

            info.keepWidth = info.keepHeight = false;
            info.Left.ToLeft = info.Right.ToRight = 0.0f;
            info.Top.ToTop = info.Bottom.ToBottom = 0.0f;

            m_layout->addElement(content, info);
        }
    }

    float TreeViewItem::ChildItemImpl::unfoldedHeight() const
    {
        return m_unfoldedHeight;
    }

    void TreeViewItem::ChildItemImpl::setUnfoldedHeight(float value)
    {
        m_unfoldedHeight = value;

        if (ptr->m_currState.ancestorUnfolded())
        {
            ptr->resize(ptr->width(), m_unfoldedHeight);
        }
    }

    const WeakPtr<TreeView>& TreeViewItem::parentView() const
    {
        return m_parentView;
    }

    void TreeViewItem::updateMasterViewConstraints()
    {
        if (!m_parentView.expired())
        {
            m_parentView.lock()->updateItemConstraints();
        }
    }

    size_t TreeViewItem::nodeLevel() const
    {
        return m_nodeLevel;
    }

    const WeakPtr<TreeViewItem>& TreeViewItem::parentItem() const
    {
        return m_parentItem;
    }

    const TreeViewItem::ChildItemImplList& TreeViewItem::childrenItems() const
    {
        return m_childrenItems;
    }

    void TreeViewItem::insertItem(const ChildItemList& items, size_t index)
    {
        // "index == m_childrenItems.size()" ---> append
        index = std::clamp(index, 0ull, m_childrenItems.size());

        auto childItor = std::next(m_childrenItems.begin(), index);

        // We must update the parent item before the raw view,
        // since the activity depends on the folding-flag of the parent item.
        for (auto& item : items)
        {
            item->m_parentItem = std::static_pointer_cast<TreeViewItem>(shared_from_this());
            item->updateMiscellaneousFields();

            auto itemItor = m_childrenItems.insert(childItor, item);
            item->m_itemImplPointer = &(*itemItor);

            if (item->m_currState.ancestorFolded())
            {
                item->resize(item->width(), 0.0f);
                item->notifyHideChildrenItems();
            }
        }
        if (!m_parentView.expired())
        {
            auto viewPtr = m_parentView.lock();

            size_t insertIndex = 0;
            if (index > 0)
            {
                // Search the child item in the global expanded list at first,
                // and then locate the insert position according to the index.
                for (auto& child : viewPtr->childrenItems())
                {
                    if (cpp_lang_utils::isMostDerivedEqual(child, childItor->ptr)) break;
                    ++insertIndex;
                }
            }
            else // Insert as the first child (immediately after the parent).
            {
                for (auto& child : viewPtr->childrenItems())
                {
                    if (cpp_lang_utils::isMostDerivedEqual(child, shared_from_this())) break;
                    ++insertIndex;
                }
                ++insertIndex;
            }
            auto rawViewPtr = (TreeView::WaterfallView*)viewPtr.get();
            rawViewPtr->insertItem(getExpandedTreeViewItems(items), insertIndex);
        }
    }

    void TreeViewItem::appendItem(const ChildItemList& items)
    {
        insertItem(items, m_childrenItems.size());
    }

    void TreeViewItem::removeItem(size_t index, size_t count)
    {
        if (index >= 0 && index < m_childrenItems.size() && count > 0)
        {
            count = std::min(count, m_childrenItems.size() - index);

            auto startChildItor = std::next(m_childrenItems.begin(), index);

            if (!m_parentView.expired())
            {
                auto viewPtr = m_parentView.lock();
                // We must create a copy here since startChildItor 
                // will be used to erase items from m_childrenItems later.
                ChildItemImplList::iterator childItor = startChildItor;

                size_t removeIndex = 0;
                for (auto& child : viewPtr->childrenItems())
                {
                    if (cpp_lang_utils::isMostDerivedEqual(child, childItor->ptr)) break;
                    ++removeIndex;
                }
                size_t removeCount = 0;
                for (size_t i = 0; i < count; ++i)
                {
                    removeCount += (childItor->ptr->getExpandedChildrenCount() + 1);
                    childItor = std::next(childItor);
                }
                auto rawViewPtr = (TreeView::WaterfallView*)viewPtr.get();
                rawViewPtr->removeItem(removeIndex, removeCount);
            }
            for (size_t i = 0; i < count; ++i)
            {
                startChildItor->ptr->m_itemImplPointer = nullptr;
                startChildItor->ptr->m_parentItem.reset();
                startChildItor->ptr->updateMiscellaneousFields();

                startChildItor = m_childrenItems.erase(startChildItor);
            }
        }
    }

    void TreeViewItem::clearAllItems()
    {
        // There is no trivial clearing since we do not know their indices in the parent view.
        removeItem(0, m_childrenItems.size());
    }

    TreeViewItem::ChildItemImpl* TreeViewItem::peekItemImpl() const
    {
        return m_itemImplPointer;
    }

    void TreeViewItem::fold()
    {
        notifyHideChildrenItems();
        updateMasterViewConstraints();
    }

    void TreeViewItem::notifyHideChildrenItems()
    {
        for (auto& item : m_childrenItems)
        {
            item.ptr->resize(item.ptr->width(), 0.0f);
            item.ptr->m_currState.ancestorFlag = FOLDED;

            item.ptr->notifyHideChildrenItems();
        }
    }

    void TreeViewItem::unfold()
    {
        notifyShowChildrenItems();
        updateMasterViewConstraints();
    }

    void TreeViewItem::notifyShowChildrenItems()
    {
        for (auto& item : m_childrenItems)
        {
            item.ptr->resize(item.ptr->width(), item.m_unfoldedHeight);
            item.ptr->m_currState.ancestorFlag = UNFOLDED;

            if (item.ptr->m_currState.folded())
            {
                item.ptr->notifyHideChildrenItems();
            }
            else // expanded item encountered
            {
                item.ptr->notifyShowChildrenItems();
            }
        }
    }

    size_t TreeViewItem::getExpandedChildrenCount() const
    {
        return getExpandedTreeViewItemCount(m_childrenItems);
    }

    size_t getExpandedTreeViewItemCount(const TreeViewItem::ChildItemList& items)
    {
        size_t count = items.size();
        for (auto& item : items)
        {
            count += item->getExpandedChildrenCount();
        }
        return count;
    }

    size_t getExpandedTreeViewItemCount(const TreeViewItem::ChildItemImplList& items)
    {
        size_t count = items.size();
        for (auto& item : items)
        {
            count += item.ptr->getExpandedChildrenCount();
        }
        return count;
    }

    TreeViewItem::ChildItemList TreeViewItem::getExpandedChildrenItems() const
    {
        return getExpandedTreeViewItems(m_childrenItems);
    }

    TreeViewItem::ChildItemList getExpandedTreeViewItems(const TreeViewItem::ChildItemList& items)
    {
        TreeViewItem::ChildItemList expandedItems = {};
        for (auto& item : items)
        {
            expandedItems.push_back(item);

            auto children = item->getExpandedChildrenItems();
            expandedItems.insert(expandedItems.end(), children.begin(), children.end());
        }
        return expandedItems;
    }

    TreeViewItem::ChildItemList getExpandedTreeViewItems(const TreeViewItem::ChildItemImplList& items)
    {
        TreeViewItem::ChildItemList expandedItems = {};
        for (auto& item : items)
        {
            expandedItems.push_back(item.ptr);

            auto children = item.ptr->getExpandedChildrenItems();
            expandedItems.insert(expandedItems.end(), children.begin(), children.end());
        }
        return expandedItems;
    }

    void TreeViewItem::updateContentHorzIndent()
    {
        updateSelfContentHorzIndent();
        updateChildrenContentHorzIndents();
    }

    void TreeViewItem::updateSelfContentHorzIndent()
    {
        auto elemItor = m_layout->findElement(content().lock());
        if (elemItor.has_value())
        {
            if (!m_parentView.expired())
            {
                auto viewPtr = m_parentView.lock();

                elemItor.value()->second.Left.ToLeft =
                    viewPtr->baseHorzIndent() +
                    viewPtr->horzIndentEachNodelLevel() * m_nodeLevel;
            }
            else elemItor.value()->second.Left.ToLeft = 0.0f;

            m_layout->updateElement(elemItor.value());
        }
    }

    void TreeViewItem::updateChildrenContentHorzIndents()
    {
        for (auto& item : m_childrenItems)
        {
            item.ptr->updateContentHorzIndent();
        }
    }

    void TreeViewItem::updateMiscellaneousFields()
    {
        updateSelfMiscellaneousFields();
        updateChildrenMiscellaneousFields();
    }

    void TreeViewItem::updateSelfMiscellaneousFields()
    {
        if (m_parentItem.expired())
        {
            m_currState.ancestorFlag = UNFOLDED;

            m_nodeLevel = 0;
            m_parentView.reset();
        }
        else // parent available
        {
            auto parentItemPtr = m_parentItem.lock();

            if (parentItemPtr->m_currState.unfolded() &&
                parentItemPtr->m_currState.ancestorUnfolded())
            {
                m_currState.ancestorFlag = UNFOLDED;
            }
            else m_currState.ancestorFlag = FOLDED;

            m_nodeLevel = parentItemPtr->m_nodeLevel + 1;
            m_parentView = parentItemPtr->m_parentView;
        }
        updateSelfContentHorzIndent();
    }

    void TreeViewItem::updateChildrenMiscellaneousFields()
    {
        for (auto& item : m_childrenItems)
        {
            item.ptr->updateMiscellaneousFields();
        }
    }

    void TreeViewItem::setFolded(StatefulObject::State::Flag flag)
    {
        StatefulObject::m_state.flag = flag;

        StatefulObject::Event soe = {};
        soe.flag = StatefulObject::m_state.flag;
        soe.ancestorFlag = m_currState.ancestorFlag;

        if (soe != m_currState)
        {
            m_currState = soe;
            onStateChange(m_currState);
        }
    }

    void TreeViewItem::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        ViewItem::onRendererDrawD2d1ObjectHelper(rndr);

        // Fold/Unfold Arrow
        if (!m_childrenItems.empty() && !m_parentView.expired())
        {
            auto& setting = getAppearance().arrow;
            auto& geoSetting = setting.geometry[StatefulObject::m_state.index()];

            resource_utils::g_solidColorBrush->SetColor(setting.background.color);
            resource_utils::g_solidColorBrush->SetOpacity(setting.background.opacity);

            auto offset = m_nodeLevel * m_parentView.lock()->horzIndentEachNodelLevel();
            auto arrowLeftTop = math_utils::offset(absolutePosition(), { offset, 0.0f });

            rndr->d2d1DeviceContext()->DrawLine(
                math_utils::offset(arrowLeftTop, geoSetting.line0.point0),
                math_utils::offset(arrowLeftTop, geoSetting.line0.point1),
                resource_utils::g_solidColorBrush.Get(), setting.strokeWidth);

            rndr->d2d1DeviceContext()->DrawLine(
                math_utils::offset(arrowLeftTop, geoSetting.line1.point0),
                math_utils::offset(arrowLeftTop, geoSetting.line1.point1),
                resource_utils::g_solidColorBrush.Get(), setting.strokeWidth);
        }
    }

    void TreeViewItem::onSizeHelper(SizeEvent& e)
    {
        ViewItem::onSizeHelper(e);

        contentMask.loadMaskBitmap(math_utils::roundu(e.size));

        m_layout->transform(selfCoordRect());
    }

    void TreeViewItem::onChangeThemeHelper(WstrParam themeName)
    {
        ViewItem::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(themeName);
    }

    void TreeViewItem::onMouseButtonHelper(MouseButtonEvent& e)
    {
        ViewItem::onMouseButtonHelper(e);

        if (!m_parentView.expired())
        {
            auto& p = e.cursorPoint;

            auto selfCoord = absoluteToSelfCoord(p);

            auto view = m_parentView.lock();
            auto areaStartX = m_nodeLevel * view->horzIndentEachNodelLevel();
            auto areaEndX = areaStartX + view->baseHorzIndent();

            // Trigger fold/unfold arrow.
            if (selfCoord.x > areaStartX && selfCoord.x < areaEndX)
            {
                if (e.state.leftDown() || e.state.leftDblclk())
                {
                    setFolded(m_currState.folded() ? UNFOLDED : FOLDED);
                }
            }
            else if (e.state.leftDblclk())
            {
                setFolded(m_currState.folded() ? UNFOLDED : FOLDED);
            }
        }
        else if (e.state.leftDblclk())
        {
            setFolded(m_currState.folded() ? UNFOLDED : FOLDED);
        }
    }

    void TreeViewItem::onStateChangeHelper(StatefulObject::Event& e)
    {
        StatefulObject::onStateChangeHelper(e);

        if (e.folded()) fold(); else unfold();

        if (!m_parentView.expired())
        {
            auto view = m_parentView.lock();
            view->updateItemIndexRangeActivity();
        }
    }
}
