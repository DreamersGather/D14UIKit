#include "Common/Precompile.h"

#include "Renderer/TickTimer.h"

#include "Common/MathUtils/Basic.h"

namespace d14engine::renderer
{
    TickTimer::TickTimer()
    {
        QueryPerformanceFrequency((LARGE_INTEGER*)&m_tickCountPerSec);
        m_secPerTickCount = 1.0 / (double)m_tickCountPerSec;

        start();
    }

    void TickTimer::start()
    {
        m_isPause = false;

        QueryPerformanceCounter((LARGE_INTEGER*)&m_baseTickCount);
        m_currTickCount = m_baseTickCount;

        m_fps = 0;
        m_frameCount = 0;
        m_updatePoint = 0.0;

        m_deltaSecs = 0.0;

        m_elapsedSecsAtLastPause = 0.0;
        m_elapsedSecsSinceResume = 0.0;
    }

    bool TickTimer::isPause() const
    {
        return m_isPause;
    }

    void TickTimer::pause()
    {
        m_isPause = true;

        m_fps = 0;
        m_frameCount = 0;
        m_updatePoint = 0.0;

        m_deltaSecs = 0.0;

        m_elapsedSecsAtLastPause = elapsedSecs();
        m_elapsedSecsSinceResume = 0.0;
    }

    void TickTimer::resume()
    {
        if (m_isPause)
        {
            m_isPause = false;
            QueryPerformanceCounter((LARGE_INTEGER*)&m_baseTickCount);
            m_currTickCount = m_baseTickCount;
        }
    }

    double TickTimer::fps() const
    {
        return m_fps;
    }

    UINT TickTimer::fpsNum() const
    {
        return math_utils::round<UINT>(m_fps);
    }

    double TickTimer::sampleInterval() const
    {
        return m_sampleInterval;
    }

    void TickTimer::setSampleInterval(double value)
    {
        m_sampleInterval = value;
    }

    void TickTimer::tick()
    {
        if (m_isPause) return;

        QueryPerformanceCounter((LARGE_INTEGER*)&m_currTickCount);
        auto tickCount = (double)(m_currTickCount - m_baseTickCount);
        auto currElapsedSecs = tickCount * m_secPerTickCount;

        m_deltaSecs = currElapsedSecs - m_elapsedSecsSinceResume;

        m_elapsedSecsSinceResume = currElapsedSecs;

        //---------------------------------------------------------------------
        // There are generally two strategies for calculating FPS:
        //---------------------------------------------------------------------
        // 1. Directly use the reciprocal of the frame generation time as FPS.
        //
        // 2. Set a sampling interval, increment the counter value each frame,
        //    and trigger when the elapsed time exceeds the sampling interval,
        //    at which point ((frame count) / (sampling interval))
        //    is the average FPS during this period. TickTimer uses this method.
        //---------------------------------------------------------------------
        // In fact, (1) is the case when the sampling time of (2) tends to zero.

        ++m_frameCount;

        auto updatePoint = m_updatePoint + m_sampleInterval;
        if (elapsedSecs() >= updatePoint)
        {
            m_fps = (double)m_frameCount / m_sampleInterval;

            m_frameCount = 0;
            m_updatePoint = elapsedSecs();
        }
    }

    UINT64 TickTimer::tickCountPerSec() const
    {
        return m_tickCountPerSec;
    }

    UINT64 TickTimer::baseTickCount() const
    {
        return m_baseTickCount;
    }

    UINT64 TickTimer::currTickCount() const
    {
        return m_currTickCount;
    }

    double TickTimer::secPerTickCount() const
    {
        return m_secPerTickCount;
    }

    double TickTimer::deltaSecs() const
    {
        return m_deltaSecs;
    }

    double TickTimer::elapsedSecsAtLastPause() const
    {
        return m_elapsedSecsAtLastPause;
    }

    double TickTimer::elapsedSecsSinceResume() const
    {
        return m_elapsedSecsSinceResume;
    }

    double TickTimer::elapsedSecs() const
    {
        return m_elapsedSecsAtLastPause + m_elapsedSecsSinceResume;
    }
}
