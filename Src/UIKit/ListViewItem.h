#pragma once

#include "Common/Precompile.h"

#include "UIKit/ViewItem.h"

namespace d14engine::uikit
{
    struct ListViewItem : ViewItem
    {
        friend struct ListView;

        using ViewItem::ViewItem;

    protected:
        WeakPtr<ListView> m_parentView = {};

    public:
        const WeakPtr<ListView>& parentView() const;
    };
}
