#pragma once

#include "ResizablePanel.h"

namespace d14uikit
{
    class DllExport GridLayout : public ResizablePanel
    {
        _D14_UIKIT_PIMPL(GridLayout)

        GridLayout();

        int horzSpacing() const;
        void setHorzSpacing(int value);

        int vertSpacing() const;
        void setVertSpacing(int value);

        int horzCellCount() const;
        void setHorzCellCount(int value);

        int vertCellCount() const;
        void setVertCellCount(int value);

        struct GeoInfo
        {
            bool fixedSize = false;

            Range x = {}, y = {};

            Rect spacing = { 0, 0, 0, 0 };
        };
        void addElement(Panel* elem, const GeoInfo& geoInfo);

        void removeElement(Panel* elem);
        void clearAllElements();

        void updateElement(Panel* elem, const GeoInfo& geoInfo);
    };
}
