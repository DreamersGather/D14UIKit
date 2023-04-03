#include "Common/Precompile.h"

#include "FilledButton.h"

#include "Button.h"
#include "ClickablePanel.h"
#include "FlatButton.h"
#include "Panel.h"

#include "UIKit/FilledButton.h"

using namespace d14engine;

namespace d14uikit
{
    FilledButton::FilledButton(const std::wstring& text)
        :
        FilledButton(Passkey{})
    {
        Panel::pimpl->uiobj =
        ClickablePanel::pimpl->uiobj =
        Button::pimpl->uiobj =
        FlatButton::pimpl->uiobj =
        FilledButton::pimpl->uiobj =
        uikit::makeUIObject<uikit::FilledButton>(text);

        Panel::initialize();
        ClickablePanel::initialize();
        Button::initialize();
        FlatButton::initialize();
        FilledButton::initialize();
    }

    FilledButton::FilledButton(Passkey)
        :
        Panel(Panel::Passkey{}),
        FlatButton(FlatButton::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void FilledButton::initialize() { }
}
