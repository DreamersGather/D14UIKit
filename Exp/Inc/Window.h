﻿#pragma once

#include "Common.h"

#include "DraggablePanel.h"
#include "ResizablePanel.h"

namespace d14uikit
{
    class DllExport Window : public DraggablePanel, public ResizablePanel
    {
        _D14_UIKIT_PIMPL_DEF

        explicit Window(const std::wstring& title = L"Untitled");

        int captionHeight() const;
        void setCaptionHeight(int value);

        int decoBarHeight() const;
        void setDecoBarHeight(int value);

        const std::wstring& title() const;
        void setTitle(const std::wstring& title);

        int contentHeight() const;
        int nonContentHeight() const;

        Panel* content() const;
        void setContent(Panel* uiobj);

        bool normal() const;
        bool minimized() const;
        bool maximized() const;

        bool minimizeButton() const;
        void setMinimizeButton(bool value);

        bool maximizeButton() const;
        void setMaximizeButton(bool value);

        bool closeButton() const;
        void setCloseButton(bool value);

        struct Callback
        {
            std::function<void(Window*)> onMinimize = {};

            std::function<void(Window*)> onMaximize = {};

            std::function<void(Window*)> onRestore = {};

            std::function<void(Window*)> onClose = {};
        };
        Callback& callback() const;

    protected:
        void initialize();
        explicit Window(Passkey);

        std::unique_ptr<Callback> pcallback = {};

        virtual void onMinimize();

        virtual void onMaximize();

        virtual void onRestore();

        virtual void onClose();
    };
}
