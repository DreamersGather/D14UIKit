#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer { struct Renderer; }

namespace d14engine::uikit::animation_utils
{
    using FrameSequence = std::vector<ComPtr<ID2D1Bitmap1>>;

    using FramePackage = std::unordered_map<Wstring, ComPtr<ID2D1Bitmap1>>;

    struct DynamicBitmap
    {
        bool visible = true;
        float opacity = 1.0f;

        // Destination rectangle in absolute coordinate.
        D2D1_RECT_F rect = {};

        FrameSequence frames = {};

        // It is recommended to enable the optimization and set the equal
        // time span if all frames share the same duration; otherwise
        // timeSpanInSecs must be populated correctly to match frames.

        struct EqualTimeSpanOptimization
        {
            bool enabled = true;
            float durationInSecs = 1.0f;
        }
        equalTimeSpanOptimization = {};

        using TimeSpanArray = std::vector<float>;

        TimeSpanArray timeSpansInSecs = {};

    private:
        size_t m_currFrameIndex = SIZE_MAX;
        float m_prevFrameElapsedSecs = 0.0f;

    public:
        void restore();

        void update(renderer::Renderer* rndr);
        void draw(renderer::Renderer* rndr);
    };
}
