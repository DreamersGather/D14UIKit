#include "Common/Precompile.h"

#include "ToggleButton.h"

#include "Common.h"

#include "UIKit/ToggleButton.h"

using namespace d14engine;

namespace d14uikit
{
    ToggleButton::ToggleButton(const std::wstring& text)
        :
        ToggleButton(uikit::makeUIObject<uikit::ToggleButton>(text)) { }

    _D14_UIKIT_CTOR(ToggleButton)
        :
        Panel(uiobj),
        FilledButton(uiobj),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>())
    {
        pimpl->uiobj = uiobj;

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
