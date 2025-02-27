#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer
{
    struct TickTimer
    {
        TickTimer();

        void start();

    private:
        bool m_isPause = false;

    public:
        bool isPause() const;

        void pause();
        void resume();

    private:
        double m_fps = 0;
        double m_sampleInterval = 1.0;

        UINT m_frameCount = 0;
        double m_updatePoint = 0.0;

    public:
        double fps() const;
        // round integer number of fps
        UINT fpsNum() const;

        double sampleInterval() const;
        void setSampleInterval(double value);

    private:
        UINT64 m_tickCountPerSec = 0;

        UINT64 m_baseTickCount = 0;
        UINT64 m_currTickCount = 0;

        double m_secPerTickCount = 0.0;

        double m_deltaSecs = 0.0;

        double m_elapsedSecsAtLastPause = 0.0;
        double m_elapsedSecsSinceResume = 0.0;

    public:
        void tick();

        UINT64 tickCountPerSec() const;

        UINT64 baseTickCount() const;
        UINT64 currTickCount() const;

        double secPerTickCount() const;

        double deltaSecs() const;

        double elapsedSecsAtLastPause() const;
        double elapsedSecsSinceResume() const;

        double elapsedSecs() const;
    };
}
