#include "Common/Precompile.h"

#include "Renderer/TickTimer.h"

namespace d14engine::renderer
{
    TickTimer::TickTimer()
    {
        QueryPerformanceFrequency((LARGE_INTEGER*)&m_tickCountPerSec);
        m_secPerTickCount = 1.0 / m_tickCountPerSec;

        start();
    }

    bool TickTimer::isPause() const
    {
        return m_isPause;
    }

    void TickTimer::start()
    {
        m_isPause = false;

        QueryPerformanceCounter((LARGE_INTEGER*)&m_baseTickCount);
        m_currTickCount = m_baseTickCount;

        m_fps = 0;
        m_frameCount = 0;
        m_oneSecPoint = 0.0;

        m_deltaSecs = 0.0;

        m_elapsedSecsAtLastPause = 0.0;
        m_elapsedSecsSinceResume = 0.0;
    }

    void TickTimer::tick()
    {
        if (m_isPause) return;

        QueryPerformanceCounter((LARGE_INTEGER*)&m_currTickCount);
        double currElapsedSecs = (m_currTickCount - m_baseTickCount) * m_secPerTickCount;

        m_deltaSecs = currElapsedSecs - m_elapsedSecsSinceResume;

        m_elapsedSecsSinceResume = currElapsedSecs;

        ++m_frameCount;
        if (elapsedSecs() >= m_oneSecPoint + 1)
        {
            m_fps = m_frameCount;
            m_frameCount = 0;
            m_oneSecPoint = elapsedSecs();
        }
    }

    void TickTimer::stop()
    {
        m_isPause = true;
        
        m_fps = 0;
        m_frameCount = 0;
        m_oneSecPoint = 0.0;

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

    UINT TickTimer::fps() const
    {
        return m_fps;
    }

    __int64 TickTimer::tickCountPerSec() const
    {
        return m_tickCountPerSec;
    }

    __int64 TickTimer::baseTickCount() const
    {
        return m_baseTickCount;
    }

    __int64 TickTimer::currTickCount() const
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
