#pragma once

#include "Panel.h"

_D14_UIKIT_FWDEF(IconLabel)

namespace d14uikit
{
    class Label;

    class DllExport IconLabel : public Panel
    {
        friend class Button;
        friend class TabCaption;
        friend class ViewItem;
        friend class Window;

        _D14_UIKIT_PIMPL(IconLabel)

        explicit IconLabel(const std::wstring& text = {});

        // The return value may be null
        Image* icon() const;
        void setIcon(Image* icon);

        Size iconSize() const;
        void setIconSize(const std::optional<Size>& value);

        Label* label() const;

        void updateLayout();
    };
}
