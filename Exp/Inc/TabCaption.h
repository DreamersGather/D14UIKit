#pragma once

#include "Panel.h"
#include "TextFormat.h"

namespace d14uikit
{
    class DllExport TabCaption : public Panel
    {
        _D14_UIKIT_PIMPL(TabCaption)

        explicit TabCaption(const std::wstring& title = L"Untitled");

        Image* icon() const;
        void setIcon(Image* icon);

        Size iconSize() const;
        void setIconSize(const std::optional<Size>& value);

        const std::wstring& title() const;
        void setTitle(const std::wstring& title);

        _D14_UIKIT_TEXT_FORMAT_DECL_CONCRETE()

        bool closable() const;
        void setClosable(bool value);

        bool draggable() const;
        void setDraggable(bool value);

        bool promotable() const;
        void setPromotable(bool value);
    };
}
