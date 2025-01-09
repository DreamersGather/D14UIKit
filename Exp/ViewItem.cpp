#include "Common/Precompile.h"

#include "ViewItem.h"

#include "Common.h"
#include "IconLabel.h"
#include "Label.h"

#include "UIKit/IconLabel.h"
#include "UIKit/ViewItem.h"

using namespace d14engine;

namespace d14uikit
{
    ViewItem::ViewItem(const std::wstring& text)
        :
        ViewItem(uikit::makeUIObject<uikit::ViewItem>(text)) { }

    _D14_UIKIT_CTOR(ViewItem)
        :
        Panel(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;

        setHeight(30);

        auto content = pimpl->uiobj->getContent<uikit::IconLabel>();
        if (!content.expired())
        {
            pimpl->content = std::shared_ptr<IconLabel>(new IconLabel(content.lock()));
        }
    }

    IconLabel* ViewItem::content() const
    {
        return pimpl->content.get();
    }

    const std::wstring& ViewItem::text() const
    {
        return pimpl->content->label()->text();
    }

    void ViewItem::setText(const std::wstring& text)
    {
        pimpl->content->label()->setText(text);
    }
}
