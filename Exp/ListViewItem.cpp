#include "Common/Precompile.h"

#include "ListViewItem.h"

#include "Panel.h"
#include "ViewItem.h"

#include "UIKit/ListViewItem.h"

using namespace d14engine;

namespace d14uikit
{
    ListViewItem::ListViewItem(const std::wstring& text)
        :
        ListViewItem(Passkey{})
    {
        Panel::pimpl->uiobj =
        ViewItem::pimpl->uiobj =
        ListViewItem::pimpl->uiobj =
        uikit::makeUIObject<uikit::ListViewItem>(text);

        Panel::initialize();
        ViewItem::initialize();
        ListViewItem::initialize();
    }

    ListViewItem::ListViewItem(Passkey)
        :
        ViewItem(ViewItem::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void ListViewItem::initialize() { }
}
