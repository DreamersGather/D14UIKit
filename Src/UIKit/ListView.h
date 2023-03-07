#pragma once

#include "Common/Precompile.h"

#include "UIKit/ListViewItem.h"
#include "UIKit/WaterfallView.h"

namespace d14engine::uikit
{
    struct ListView : WaterfallView<ListViewItem>
    {
        explicit ListView(const D2D1_RECT_F& rect = {});

        void insertItem(const ItemList& items, size_t index = 0) override;
        void appendItem(const ItemList& items);

        void removeItem(size_t index, size_t count = 1) override;
        void clearAllItems() override;
    };
}
