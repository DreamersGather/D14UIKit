#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/TreeViewItem.h"
#include "UIKit/StatefulObject.h"
#include "UIKit/ViewItem.h"

namespace d14engine::uikit
{
    struct ConstraintLayout;

    struct TreeViewItem
        :
        appearance::TreeViewItem, ViewItem,
        StatefulObject<TreeViewItemState, TreeViewItemStateChangeEvent>
    {
        friend struct TreeView;

        TreeViewItem(ShrdPtrParam<Panel> content, const D2D1_RECT_F& rect = {});

        TreeViewItem(WstrParam text = L"ViewItem", const D2D1_RECT_F& rect = {});

        using ChildItemList = std::list<SharedPtr<TreeViewItem>>;

        void onInitializeFinish() override;

        struct ArrowIcon
        {
            ComPtr<ID2D1StrokeStyle> strokeStyle = {};
        }
        arrowIcon = {};

        void loadArrowIconStrokeStyle();

        _D14_SET_APPEARANCE_GETTER(TreeViewItem)

    protected:
        SharedPtr<ConstraintLayout> m_layout = {};

    public:
        template<typename T = Panel>
        WeakPtr<T> getContent() const
        {
            if constexpr (std::is_same_v<T, Panel>) return content();
            else return std::dynamic_pointer_cast<T>(content().lock());
        }
        WeakPtr<Panel> content() const;
        void setContent(ShrdPtrParam<Panel> content);

    protected:
        WeakPtr<TreeView> m_parentView = {};

        // For example:
        //
        // Root---Child_0---Child_1--Child_2
        //           |         |
        //           |         \---Child_10
        //           |
        //           \---Child_00---Child_01
        //
        // where the node levels are:
        //
        // [0, parent       ] Root
        // [1, children     ] Child_0, Child_1, Child_2
        // [2, grandchildren] Child_00, Child_01, Child_10
        size_t m_nodeLevel = 0;

        WeakPtr<TreeViewItem> m_parentItem = {};

        struct ChildItemImpl
        {
            friend struct TreeViewItem;

            ChildItemImpl(ShrdPtrParam<TreeViewItem> rhs)
                : ptr(rhs), m_unfoldedHeight(rhs->height()) { }

            SharedPtr<TreeViewItem> ptr = {};

        private:
            // Since the height of a folded item will be set to 0, we need
            // this backup to help restore the height after the item unfolded.
            float m_unfoldedHeight = {};

        public:
            float unfoldedHeight() const;

            // Note this method only update the item and the backup, so you
            // need to call updateMasterViewConstraints later to update the
            // appearance immediately.
            //
            // We do not call updateMasterViewConstraints in this method
            // since it can cause unnecessary performance loss when the
            // setter is called more than once, so basically you should
            // iterate all the candidate items firstly to set their heights
            // and then call updateMasterViewConstraints finally.
            void setUnfoldedHeight(float value);
        };
        using ChildItemImplList = std::list<ChildItemImpl>;

        ChildItemImplList m_childrenItems = {};

        // Points to ChildItemImpl maintained in ChildItemImplList of the
        // parent item (however, always equals nullptr for any root-item).
        ChildItemImpl* m_itemImplPointer = nullptr;

    public:
        const WeakPtr<TreeView>& parentView() const;

        void updateMasterViewConstraints();

        size_t nodeLevel() const;

        const WeakPtr<TreeViewItem>& parentItem() const;

        const ChildItemImplList& childrenItems() const;

        void insertItem(const ChildItemList& items, size_t index = 0);
        void appendItem(const ChildItemList& items);

        void removeItem(size_t index, size_t count = 1);
        void clearAllItems();

        ChildItemImpl* peekItemImpl() const;

    protected:
        void fold(); void notifyHideChildrenItems();
        void unfold(); void notifyShowChildrenItems();

        // Convert item-tree to item-list:
        //
        // Root---Child_0---Child_1--Child_2
        //           |         |
        //           |         \---Child_10
        //           |
        //           \---Child_00---Child_01
        //
        // will be expanded to:
        //
        // Root---Child_0---Child_00---Child_01---Child_1---Child_10---Child_2

        size_t getExpandedChildrenCount() const;

        friend size_t getExpandedTreeViewItemCount(const ChildItemList& items);
        friend size_t getExpandedTreeViewItemCount(const ChildItemImplList& items);

        ChildItemList getExpandedChildrenItems() const;

        friend ChildItemList getExpandedTreeViewItems(const ChildItemList& items);
        friend ChildItemList getExpandedTreeViewItems(const ChildItemImplList& items);

        void updateContentHorzIndent();
        void updateSelfContentHorzIndent();
        void updateChildrenContentHorzIndents();

        void updateMiscellaneousFields();
        void updateSelfMiscellaneousFields();
        void updateChildrenMiscellaneousFields();

    public:
        constexpr static auto FOLDED = StatefulObject::State::Flag::Folded;
        constexpr static auto UNFOLDED = StatefulObject::State::Flag::Unfolded;

        void setFolded(StatefulObject::State::Flag flag);

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        void onSizeHelper(SizeEvent& e) override;

        void onChangeThemeHelper(WstrParam themeName) override;

        void onMouseButtonHelper(MouseButtonEvent& e) override;

        // StatefulObject
        void onStateChangeHelper(StatefulObject::Event& e) override;
    };
}
