#include "Common/Precompile.h"

#include "TabCaption.h"

#include "Common.h"
#include "IconLabel.h"
#include "Label.h"

#include "UIKit/IconLabel.h"
#include "UIKit/TabCaption.h"

using namespace d14engine;

namespace d14uikit
{
    TabCaption::TabCaption(const std::wstring& title)
        :
        TabCaption(uikit::makeUIObject<uikit::TabCaption>(title)) { }

    _D14_UIKIT_CTOR(TabCaption)
        :
        Panel(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;

        _D14_UIKIT_BIND(IconLabel, title);
    }

    IconLabel* TabCaption::title() const
    {
        return pimpl->title.get();
    }

    const std::wstring& TabCaption::text() const
    {
        return pimpl->title->label()->text();
    }

    void TabCaption::setText(const std::wstring& text)
    {
        pimpl->title->label()->setText(text);
    }

    bool TabCaption::closable() const
    {
        return pimpl->uiobj->closable;
    }

    void TabCaption::setClosable(bool value)
    {
        pimpl->uiobj->closable = value;
    }

    bool TabCaption::draggable() const
    {
        return pimpl->uiobj->draggable;
    }

    void TabCaption::setDraggable(bool value)
    {
        pimpl->uiobj->draggable = value;
    }

    bool TabCaption::promotable() const
    {
        return pimpl->uiobj->promotable;
    }

    void TabCaption::setPromotable(bool value)
    {
        pimpl->uiobj->promotable = value;
    }
}
