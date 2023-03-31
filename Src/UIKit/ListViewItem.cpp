#include "Common/Precompile.h"

#include "UIKit/ListViewItem.h"

namespace d14engine::uikit
{
    const WeakPtr<ListView>& ListViewItem::parentView() const
    {
        return m_parentView;
    }
}
