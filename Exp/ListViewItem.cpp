#include "Common/Precompile.h"

#include "ListViewItem.h"

#include "Common.h"

#include "UIKit/ListViewItem.h"

using namespace d14engine;

namespace d14uikit
{
    ListViewItem::ListViewItem(const std::wstring& text)
        :
        ListViewItem(uikit::makeUIObject<uikit::ListViewItem>(text)) { }

    _D14_UIKIT_CTOR(ListViewItem)
        :
        ViewItem(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }
}
