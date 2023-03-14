#include "Common/Precompile.h"

#include "ClickablePanel.h"
#include "Button.h"
#include "FlatButton.h"
#include "Panel.h"

#include "UIKit/FlatButton.h"

using namespace d14engine;

namespace d14uikit
{
    FlatButton::FlatButton(const std::wstring& text)
        :
        FlatButton(Passkey{})
    {
        Panel::pimpl->uiobj =
        ClickablePanel::pimpl->uiobj =
        Button::pimpl->uiobj =
        FlatButton::pimpl->uiobj =
        uikit::makeUIObject<uikit::FlatButton>(text);

        Panel::initialize();
        ClickablePanel::initialize();
        Button::initialize();
        FlatButton::initialize();
    }

    FlatButton::FlatButton(Passkey)
        :
        Panel(Panel::Passkey{}),
        Button(Button::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void FlatButton::initialize() { }
}
