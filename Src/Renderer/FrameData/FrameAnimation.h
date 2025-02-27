#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer
{
    namespace fanim_literals
    {
        // 1 Jiffies == 1/60 Seconds
        constexpr float operator""_jf(unsigned long long n)
        {
            return (float)n / 60.0f;
        }
    }
    template<typename FrameType>
    struct FrameAnimation
    {
        using FrameArray = std::vector<FrameType>;
        using FramePackage = std::unordered_map<Wstring, FrameType>;

        FrameArray frames = {};

        using TimeSpanData = Variant<float, std::vector<float>>;
        constexpr static size_t g_equalTimeSpan = 0, g_variousTimeSpan = 1;

        TimeSpanData timeSpanDataInSecs = 1.0f;

    private:
        size_t m_currFrameIndex = 0;
        float m_prevFrameElapsedSecs = 0.0f;

    public:
        Optional<size_t> currFrameIndex() const
        {
            if (m_currFrameIndex >= frames.size())
            {
                return std::nullopt;
            }
            else return m_currFrameIndex;
        }

        float prevFrameElapsedSecs() const
        {
            return m_prevFrameElapsedSecs;
        }

    public:
        void restore()
        {
            m_currFrameIndex = 0;
            m_prevFrameElapsedSecs = 0.0f;
        }

        void update(float elapsedSecs)
        {
            if (!frames.empty())
            {
                float ts = 1.0f;

                if (timeSpanDataInSecs.index() == g_equalTimeSpan)
                {
                    ts = std::get<g_equalTimeSpan>(timeSpanDataInSecs);
                }
                else if (timeSpanDataInSecs.index() == g_variousTimeSpan)
                {
                    auto& data = std::get<g_variousTimeSpan>(timeSpanDataInSecs);
                    ts = m_currFrameIndex < data.size() ? data[m_currFrameIndex] : 1.0f;
                }
                m_prevFrameElapsedSecs += elapsedSecs;

                if (m_prevFrameElapsedSecs >= ts)
                {
                    m_currFrameIndex = (m_currFrameIndex + 1) % frames.size();
                    m_prevFrameElapsedSecs = 0.0f;
                }
            }
        }
    };
}
