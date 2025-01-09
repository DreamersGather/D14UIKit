#include "Common/Precompile.h"

#include "Button.h"

#include "Common.h"
#include "IconLabel.h"
#include "Label.h"

#include "UIKit/Button.h"
#include "UIKit/IconLabel.h"

using namespace d14engine;

namespace d14uikit
{
    Button::Button(const std::wstring& text)
        :
        Button(uikit::makeUIObject<uikit::Button>(text)) { }

    _D14_UIKIT_CTOR(Button)
        :
        Panel(uiobj),
        ClickablePanel(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;

        _D14_UIKIT_BIND(IconLabel, content);
    }

    IconLabel* Button::content() const
    {
        return pimpl->content.get();
    }

    const std::wstring& Button::text() const
    {
        return pimpl->content->label()->text();
    }

    void Button::setText(const std::wstring& text)
    {
        pimpl->content->label()->setText(text);
    }
}
