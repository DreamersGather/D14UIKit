#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/TypeTraits.h"

namespace d14engine::uikit
{
    template<typename State_T, typename StateChangeEvent_T>
    struct StatefulObject
    {
        static_assert(cpp_lang_utils::has_operator::equalTo<StateChangeEvent_T>,
            "An equal-to operator must be implemented for StateChangeEvent_T");

        using State = State_T;
        using Event = StateChangeEvent_T;

    public:
        void onStateChange(StateChangeEvent_T& e)
        {
            onStateChangeHelper(e);

            if (f_onStateChange) f_onStateChange(this, e);
        }

        Function<void(StatefulObject*, StateChangeEvent_T&)> f_onStateChange = {};

    protected:
        virtual void onStateChangeHelper(StateChangeEvent_T& e) { }

    protected:
        State_T m_state = {};
        StateChangeEvent_T m_currState = {};

    public:
        const StateChangeEvent_T& currState() const { return m_currState; }

        void forceUpdateCurrState() { onStateChange(m_currState); }
    };
}
