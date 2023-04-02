#pragma once

#include "Common/Precompile.h"

#include "UIKit/TreeViewItem.h"
#include "UIKit/WaterfallView.h"

namespace d14engine::uikit
{
    struct TreeView : WaterfallView<TreeViewItem>
    {
        explicit TreeView(const D2D1_RECT_F& rect = {});

        void onInitializeFinish() override;

    protected:
        ItemList m_rootItems = {};

    public:
        const ItemList& rootItems() const;

        void insertRootItem(const ItemList& items, size_t rootIndex = 0);
        void appendRootItem(const ItemList& items);

        void removeRootItem(size_t rootIndex, size_t count = 1);
        void clearAllItems() override;

    protected:
        float m_baseHorzIndent = 24.0f;

        float m_horzIndentEachNodeLevel = 24.0f;

    public:
        float baseHorzIndent() const;
        void setBaseHorzIndent(float value);

        float horzIndentEachNodelLevel() const;
        void setHorzIndentEachNodelLevel(float value);

    protected:
        // You can insert/erase only root items with the master tree-view.
        // To insert/erase child-items, you should use their parent items.
        using WaterfallView::insertItem;
        using WaterfallView::removeItem;

        void setItemIndexRangeActive(bool value) override;

        ItemIndex getRootItemGlobalIndex(size_t rootIndex,
            Optional<ItemIndex> start = std::nullopt) const;
    };
}
