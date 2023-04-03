#include "Common/Precompile.h"

#include "ElevatedButton.h"

#include "Button.h"
#include "ClickablePanel.h"
#include "FilledButton.h"
#include "FlatButton.h"
#include "Panel.h"

#include "UIKit/ElevatedButton.h"

using namespace d14engine;

namespace d14uikit
{
    ElevatedButton::ElevatedButton(const std::wstring& text)
        :
        ElevatedButton(Passkey{})
    {
        Panel::pimpl->uiobj =
        ClickablePanel::pimpl->uiobj =
        Button::pimpl->uiobj =
        FlatButton::pimpl->uiobj =
        FilledButton::pimpl->uiobj =
        ElevatedButton::pimpl->uiobj =
        uikit::makeUIObject<uikit::ElevatedButton>(text);

        Panel::initialize();
        ClickablePanel::initialize();
        Button::initialize();
        FlatButton::initialize();
        FilledButton::initialize();
        ElevatedButton::initialize();
    }

    ElevatedButton::ElevatedButton(Passkey)
        :
        Panel(Panel::Passkey{}),
        FilledButton(FilledButton::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void ElevatedButton::initialize() { }
}
