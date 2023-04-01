#pragma once

#include "Common/Precompile.h"

#include "UIKit/ViewItem.h"

namespace d14engine::uikit
{
    struct ListViewItem : ViewItem
    {
        using ViewItem::ViewItem;

        friend struct ListView;

        const WeakPtr<ListView>& parentView() const
        {
            return m_parentView;
        }
    protected:
        WeakPtr<ListView> m_parentView = {};
    };
}
