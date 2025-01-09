#include "Common/Precompile.h"

#include "OnOffSwitch.h"

#include "Common.h"

#include "UIKit/OnOffSwitch.h"

using namespace d14engine;

namespace d14uikit
{
    OnOffSwitch::OnOffSwitch()
        :
        OnOffSwitch(uikit::makeUIObject<uikit::OnOffSwitch>()) { }

    _D14_UIKIT_CTOR(OnOffSwitch)
        :
        Panel(uiobj),
        ClickablePanel(uiobj),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>())
    {
        pimpl->uiobj = uiobj;

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
