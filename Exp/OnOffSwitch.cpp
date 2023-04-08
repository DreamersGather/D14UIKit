#include "Common/Precompile.h"

#include "OnOffSwitch.h"

#include "ClickablePanel.h"
#include "Panel.h"

#include "UIKit/OnOffSwitch.h"

using namespace d14engine;

namespace d14uikit
{
    OnOffSwitch::OnOffSwitch()
        :
        OnOffSwitch(Passkey{})
    {
        Panel::pimpl->uiobj =
        ClickablePanel::pimpl->uiobj =
        OnOffSwitch::pimpl->uiobj =
        uikit::makeUIObject<uikit::OnOffSwitch>();

        Panel::initialize();
        ClickablePanel::initialize();
        OnOffSwitch::initialize();
    }

    OnOffSwitch::OnOffSwitch(Passkey)
        :
        Panel(Panel::Passkey{}),
        ClickablePanel(ClickablePanel::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void OnOffSwitch::initialize()
    {
        pimpl->uiobj->f_onStateChange = [this]
        (uikit::OnOffSwitch::StatefulObject* obj,
         uikit::OnOffSwitch::StatefulObject::Event& e)
        {
            auto state = (State)e.flag;
            onStateChange(state);
            if (pcallback->onStateChange)
            {
                pcallback->onStateChange(this, state);
            }
        };
    }

    OnOffSwitch::State OnOffSwitch::state() const
    {
        return (State)pimpl->uiobj->currState().flag;
    }

    void OnOffSwitch::setState(State state)
    {
        auto flag = (uikit::OnOffSwitchState::ActiveFlag)state;
        pimpl->uiobj->setOnOff(flag);
    }

    OnOffSwitch::Callback& OnOffSwitch::callback() const { return *pcallback; }

    void OnOffSwitch::onStateChange(State state) { } 
}
