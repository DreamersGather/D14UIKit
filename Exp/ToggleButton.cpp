#include "Common/Precompile.h"

#include "ToggleButton.h"

#include "Button.h"
#include "ClickablePanel.h"
#include "FilledButton.h"
#include "FlatButton.h"
#include "Panel.h"

#include "UIKit/ToggleButton.h"

using namespace d14engine;

namespace d14uikit
{
    ToggleButton::ToggleButton(const std::wstring& text)
        :
        ToggleButton(Passkey{})
    {
        Panel::pimpl->uiobj =
        ClickablePanel::pimpl->uiobj =
        Button::pimpl->uiobj =
        FlatButton::pimpl->uiobj =
        FilledButton::pimpl->uiobj =
        ToggleButton::pimpl->uiobj =
        uikit::makeUIObject<uikit::ToggleButton>(text);

        Panel::initialize();
        ClickablePanel::initialize();
        Button::initialize();
        FlatButton::initialize();
        FilledButton::initialize();
        ToggleButton::initialize();
    }

    ToggleButton::ToggleButton(Passkey)
        :
        Panel(Panel::Passkey{}),
        FilledButton(FilledButton::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void ToggleButton::initialize()
    {
        pimpl->uiobj->f_onStateChange = [this]
        (uikit::ToggleButton::StatefulObject* obj,
         uikit::ToggleButton::StatefulObject::Event& e)
        {
            auto state = (State)e.flag;
            onStateChange(state);
            if (pcallback->onStateChange)
            {
                pcallback->onStateChange(this, state);
            }
        };
    }

    ToggleButton::State ToggleButton::state() const
    {
        return (State)pimpl->uiobj->currState().flag;
    }

    void ToggleButton::setState(State state)
    {
        auto flag = (uikit::ToggleButtonState::ActiveFlag)state;
        pimpl->uiobj->setActivated(flag);
    }

    ToggleButton::Callback& ToggleButton::callback() const { return *pcallback; }

    void ToggleButton::onStateChange(State state) { }
}
