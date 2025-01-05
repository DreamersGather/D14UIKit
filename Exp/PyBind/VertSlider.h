#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/VertSlider.h"

namespace d14uikit
{
    void Py_InitVertSlider(py::module_& m);

    class ExVertSlider : public VertSlider
    {
    public:
        using VertSlider::onValueChange;
        using VertSlider::onEndSliding;
        using VertSlider::onStartSliding;
    };

    template<typename VertSliderBase = VertSlider>
    class PyVertSlider : public PyPanel<VertSliderBase>
    {
    public:
        using PyPanel<VertSliderBase>::PyPanel;

        void onValueChange(float value) override
        {
            PYBIND11_OVERRIDE(void, VertSliderBase, onValueChange, value);
        }
        void onEndSliding(float value) override
        {
            PYBIND11_OVERRIDE(void, VertSliderBase, onEndSliding, value);
        }
        void onStartSliding(float value) override
        {
            PYBIND11_OVERRIDE(void, VertSliderBase, onStartSliding, value);
        }
    };
}
