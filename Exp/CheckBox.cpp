#include "Common/Precompile.h"

#include "CheckBox.h"

#include "Common.h"

#include "UIKit/CheckBox.h"

using namespace d14engine;

namespace d14uikit
{
    CheckBox::CheckBox(bool tripleState)
        :
        CheckBox(uikit::makeUIObject<uikit::CheckBox>(tripleState)) { }

    _D14_UIKIT_CTOR(CheckBox)
        :
        Panel(uiobj),
        ClickablePanel(uiobj),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>())
    {
        pimpl->uiobj = uiobj;

        pimpl->uiobj->f_onStateChange = [this]
        (uikit::CheckBox::StatefulObject* obj,
         uikit::CheckBox::StatefulObject::Event& e)
        {
            auto state = (State)e.flag;
            onStateChange(state);
            if (pcallback->onStateChange)
            {
                pcallback->onStateChange(this, state);
            }
        };
    }

    CheckBox::State CheckBox::state() const
    {
        return (State)pimpl->uiobj->currState().flag;
    }

    void CheckBox::setState(State state)
    {
        auto flag = (uikit::CheckBoxState::ActiveFlag)state;
        pimpl->uiobj->setChecked(flag);
    }

    bool CheckBox::tripleState() const
    {
        return pimpl->uiobj->isTripleState();
    }

    void CheckBox::setTripleState(bool value)
    {
        pimpl->uiobj->enableTripleState(value);
    }

    CheckBox::Callback& CheckBox::callback() const { return *pcallback; }

    void CheckBox::onStateChange(State state) { }
}
