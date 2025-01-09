#pragma once

#include "Panel.h"

_D14_UIKIT_FWDEF(TabCaption)

namespace d14uikit
{
    class IconLabel;

    class DllExport TabCaption : public Panel
    {
        friend class TabGroup;

        _D14_UIKIT_PIMPL(TabCaption)

        explicit TabCaption(const std::wstring& title = L"Untitled");

        IconLabel* title() const;

        const std::wstring& text() const;
        void setText(const std::wstring& text);

        bool closable() const;
        void setClosable(bool value);

        bool draggable() const;
        void setDraggable(bool value);

        bool promotable() const;
        void setPromotable(bool value);
    };
}
