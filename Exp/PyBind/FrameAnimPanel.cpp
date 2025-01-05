#include "Common/Precompile.h"

#include "FrameAnimPanel.h"

#include "Panel.h"

#include "Inc/FrameAnimPanel.h"
#include "Inc/Image.h"

namespace d14uikit
{
    void Py_InitFrameAnimPanel(py::module_& m)
    {
        py::class_<FrameAnimPanel, Panel, PyPanel<FrameAnimPanel>> i(m, "FrameAnimPanel");

        i.def(py::init());

        i.def(
            "resetFrameState",
            &FrameAnimPanel::resetFrameState);

        i.def_property(
            "frames",
            &FrameAnimPanel::frames,
            &FrameAnimPanel::setFrames);

        i.def_property(
            "frameOpacity",
            &FrameAnimPanel::frameOpacity,
            &FrameAnimPanel::setFrameOpacity);

        i.def_property(
            "frameInterpMode",
            &FrameAnimPanel::frameInterpMode,
            &FrameAnimPanel::setFrameInterpMode);

        i.def_property(
            "frameTimeSpan",
            &FrameAnimPanel::frameTimeSpan,
            &FrameAnimPanel::setFrameTimeSpan);
    }
}
