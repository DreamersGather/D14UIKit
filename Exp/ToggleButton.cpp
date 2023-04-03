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
            State state = {};
            switch (e.flag)
            {
            case uikit::ToggleButton::ACTIVATED: state = Activated; break;
            case uikit::ToggleButton::DEACTIVATED: state = Deactivated; break;
            default: break;
            }
            onStateChange(state);
            if (pcallback->onStateChange)
            {
                pcallback->onStateChange(this, state);
            }
        };
    }

    ToggleButton::State ToggleButton::state() const
    {
        return pimpl->state;
    }

    void ToggleButton::setState(State state)
    {
        pimpl->state = state;
        uikit::ToggleButtonState::ActiveFlag flag = {};
        switch (state)
        {
        case Activated: flag = uikit::ToggleButton::ACTIVATED; break;
        case Deactivated: flag = uikit::ToggleButton::DEACTIVATED; break;
        default: break;
        }
        pimpl->uiobj->setActivated(flag);
    }

    ToggleButton::Callback& ToggleButton::callback() const { return *pcallback; }

    void ToggleButton::onStateChange(State state) { }
}
