#include "Common/Precompile.h"

#include "UIKit/ListViewItem.h"

namespace d14engine::uikit
{
    const WeakPtr<ListView>& ListViewItem::parentView() const
    {
        return m_parentView;
    }

    void ListViewItem::setEnabled(bool value)
    {
        ViewItem::setEnabled(value);

        if (m_content) m_content->setEnabled(value);
    }
}
