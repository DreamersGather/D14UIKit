﻿#pragma once

#include "ResizablePanel.h"

_D14_UIKIT_FWDEF(TabGroup)

namespace d14uikit
{
    class TabCaption;

    class DllExport TabGroup : public ResizablePanel
    {
        _D14_UIKIT_PIMPL(TabGroup)

        TabGroup();

        Size cardSize() const;
        void setCardSize(const Size& value);

        struct Tab
        {
            TabCaption* caption = nullptr;
            Panel* content = nullptr;
        };
        void insertTab(const Tab& tab, int index = 0);
        void appendTab(const Tab& tab);

        void removeTab(int index, int count = 1);
        void clearAllTabs();

        int tabCount() const;

        std::optional<Tab> currSelected() const;
        void setCurrSelected(int index);

        // Use wstring_view to avoid the overhead of copying strings
        std::optional<std::wstring_view> currSelectedTitle() const;

        struct Callback
        {
            std::function<void(TabGroup*, const std::wstring&)> onSelectedChange = {};
        };
        Callback& callback() const;

    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void onSelectedChange(const std::wstring& title);
    };
}
