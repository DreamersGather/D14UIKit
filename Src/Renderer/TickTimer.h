#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer
{
    struct TickTimer
    {
        TickTimer();

    private:
        bool m_isPause = false;

    public:
        bool isPause() const;

        void start();
        void tick();
        void stop();
        void resume();

    private:
        UINT m_fps = 0;
        UINT m_frameCount = 0;
        double m_oneSecPoint = 0.0;

    public:
        UINT fps() const;

    private:
        __int64 m_tickCountPerSec = 0;

        __int64 m_baseTickCount = 0;
        __int64 m_currTickCount = 0;

        double m_secPerTickCount = 0.0;

        double m_deltaSecs = 0.0;

        double m_elapsedSecsAtLastPause = 0.0;
        double m_elapsedSecsSinceResume = 0.0;

    public:
        __int64 tickCountPerSec() const;

        __int64 baseTickCount() const;
        __int64 currTickCount() const;

        double secPerTickCount() const;

        double deltaSecs() const;

        double elapsedSecsAtLastPause() const;
        double elapsedSecsSinceResume() const;

        double elapsedSecs() const;
    };
}
