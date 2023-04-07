#pragma once

#include "Common/Precompile.h"

#include "Panel.h"

#include "Inc/HorzSlider.h"

namespace d14uikit
{
    void Py_InitHorzSlider(py::module_& m);

    class ExHorzSlider : public HorzSlider
    {
    public:
        using HorzSlider::onValueChange;
        using HorzSlider::onEndSliding;
        using HorzSlider::onStartSliding;
    };

    template<typename HorzSliderBase = HorzSlider>
    class PyHorzSlider : public PyPanel<HorzSliderBase>
    {
    public:
        using PyPanel::PyPanel;

        void onValueChange(float value) override
        {
            PYBIND11_OVERRIDE(void, HorzSliderBase, onValueChange, value);
        }
        void onEndSliding(float value) override
        {
            PYBIND11_OVERRIDE(void, HorzSliderBase, onEndSliding, value);
        }
        void onStartSliding(float value) override
        {
            PYBIND11_OVERRIDE(void, HorzSliderBase, onStartSliding, value);
        }
    };
}
