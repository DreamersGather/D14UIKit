#include "Common/Precompile.h"

#include "Renderer/InfoUtils.h"

#include "Common/MathUtils/GDI.h"

namespace d14engine::renderer::info_utils
{
    Wstring text(const DXGI_ADAPTER_DESC& desc)
    {
        return desc.Description;
    }

    Wstring text(const DXGI_MODE_DESC& desc)
    {
        auto refreshRate =
            desc.RefreshRate.Numerator /
            desc.RefreshRate.Denominator;

        return // width x height @ refreshRate Hz
            std::to_wstring(desc.Width) + L"x" +
            std::to_wstring(desc.Height) + L" @ " +
            std::to_wstring(refreshRate) + L" Hz";
    }

    Wstring text(const DXGI_OUTPUT_DESC& desc)
    {
        auto monitor = std::wstring(desc.DeviceName);

        auto& rect = desc.DesktopCoordinates;
        auto width = math_utils::width(rect);
        auto height = math_utils::height(rect);

        return // monitor @ width x height
            monitor + L" @ " +
            std::to_wstring(width) + L"x" +
            std::to_wstring(height);
    }
}
