#include "Common/Precompile.h"

#include "OutlinedButton.h"

#include "Button.h"
#include "ClickablePanel.h"
#include "FlatButton.h"
#include "Panel.h"

#include "UIKit/OutlinedButton.h"

using namespace d14engine;

namespace d14uikit
{
    OutlinedButton::OutlinedButton(const std::wstring& text)
        :
        OutlinedButton(Passkey{})
    {
        Panel::pimpl->uiobj =
        ClickablePanel::pimpl->uiobj =
        Button::pimpl->uiobj =
        FlatButton::pimpl->uiobj =
        OutlinedButton::pimpl->uiobj =
        uikit::makeUIObject<uikit::OutlinedButton>(text);

        Panel::initialize();
        ClickablePanel::initialize();
        Button::initialize();
        FlatButton::initialize();
        OutlinedButton::initialize();
    }

    OutlinedButton::OutlinedButton(Passkey)
        :
        Panel(Panel::Passkey{}),
        FlatButton(FlatButton::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void OutlinedButton::initialize() { }
}
