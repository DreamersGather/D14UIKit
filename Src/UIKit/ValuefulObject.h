#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit
{
    template<typename Value_T>
    struct ValuefulObject
    {
        using Value = Value_T;

    protected:
        Value_T m_value = {};
        Value_T m_minValue = {};
        Value_T m_maxValue = {};

    public:
        void onValueChange(Value_T value)
        {
            onValueChangeHelper(value);

            if (f_onValueChange) f_onValueChange(this, value);
        }

        Function<void(ValuefulObject*, Value_T)> f_onValueChange = {};

    protected:
        virtual void onValueChangeHelper(Value_T value) { }

    public:
        Value_T value() const { return m_value; }
        Value_T minValue() const { return m_minValue; }
        Value_T maxValue() const { return m_maxValue; }

        // Return whether current value is changed after calling.

        virtual bool setValue(Value_T value)
        {
            if (value >= m_minValue && value <= m_maxValue)
            {
                if (value != m_value)
                {
                    m_value = value;
                    return true;
                }
            }
            return false;
        }
        virtual bool setMinimalValue(Value_T value)
        {
            if (value <= m_maxValue)
            {
                m_minValue = value;
                if (m_minValue > m_value)
                {
                    m_value = m_minValue;
                    return true;
                }
            }
            return false;
        }
        virtual bool setMaximalValue(Value_T value)
        {
            if (value >= m_minValue)
            {
                m_maxValue = value;
                if (m_maxValue < m_value)
                {
                    m_value = m_maxValue;
                    return true;
                }
            }
            return false;
        }
    };
}
