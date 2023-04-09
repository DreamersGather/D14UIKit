#include "Common/Precompile.h"

#include "Window.h"

#include "Common.h"
#include "DraggablePanel.h"
#include "Image.h"
#include "Panel.h"
#include "ResizablePanel.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/Window.h"

using namespace d14engine;

namespace d14uikit
{
    Window::Window(const std::wstring& title)
        :
        Window(Passkey{})
    {
        Window::pimpl->uiobj =
        uikit::makeUIObject
        <uikit::Window>(title);

        Panel::pimpl->uiobj = pimpl->uiobj;
        DraggablePanel::pimpl->uiobj = pimpl->uiobj;
        ResizablePanel::pimpl->uiobj = pimpl->uiobj;

        Panel::initialize();
        DraggablePanel::initialize();
        ResizablePanel::initialize();
        Window::initialize();
    }

    Window::Window(Passkey)
        :
        Panel(Panel::Passkey{}),
        DraggablePanel(DraggablePanel::Passkey{}),
        ResizablePanel(ResizablePanel::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void Window::initialize()
    {
        pimpl->uiobj->f_onMinimize = [this]
        (uikit::Window* w)
        {
            onMinimize();
            if (pcallback->onMinimize)
            {
                pcallback->onMinimize(this);
            }
        };
        pimpl->uiobj->f_onMaximize = [this]
        (uikit::Window* w)
        {
            onMaximize();
            if (pcallback->onMaximize)
            {
                pcallback->onMaximize(this);
            }
        };
        pimpl->uiobj->f_onRestore = [this]
        (uikit::Window* w)
        {
            onRestore();
            if (pcallback->onRestore)
            {
                pcallback->onRestore(this);
            }
        };
        pimpl->uiobj->f_onClose = [this]
        (uikit::Window* w)
        {
            onClose();
            if (pcallback->onClose)
            {
                pcallback->onClose(this);
            }
        };
    }

    int Window::captionHeight() const
    {
        return math_utils::round(pimpl->uiobj->captionPanelHeight());
    }

    void Window::setCaptionHeight(int value)
    {
        pimpl->uiobj->setCaptionPanelHeight((float)value);
    }

    int Window::decoBarHeight() const
    {
        return math_utils::round(pimpl->uiobj->decorativeBarHeight());
    }

    void Window::setDecoBarHeight(int value)
    {
        pimpl->uiobj->setDecorativeBarHeight((float)value);
    }

    Image* Window::icon() const
    {
        return pimpl->icon;
    }

    void Window::setIcon(Image* icon)
    {
        pimpl->icon = icon;
        auto& targetIcon = pimpl->uiobj->caption()->icon;
        if (icon != nullptr && !icon->cpuRead())
        {
            targetIcon.bitmap = icon->getImpl()->bitmap;
        }
        else targetIcon.bitmap.Reset();

        pimpl->uiobj->caption()->updateLayout();
    }

    Size Window::iconSize() const
    {
        auto& icon = pimpl->uiobj->caption()->icon;

        D2D1_SIZE_F iconSize = { 0.0f, 0.0f };
        if (icon.customSize.has_value())
        {
            iconSize = icon.customSize.value();
        }
        else if (icon.bitmap != nullptr)
        {
            iconSize = icon.bitmap->GetSize();
        }
        return convert(iconSize);
    }

    void Window::setIconSize(const std::optional<Size>& value)
    {
        auto& icon = pimpl->uiobj->caption()->icon;

        if (value.has_value())
        {
            icon.customSize = convert(value.value());
        }
        else icon.customSize.reset();

        pimpl->uiobj->caption()->updateLayout();
    }

    const std::wstring& Window::title() const
    {
        return pimpl->uiobj->caption()->label()->text();
    }

    void Window::setTitle(const std::wstring& title)
    {
        pimpl->uiobj->caption()->label()->setText(title);
    }

    int Window::nonContentHeight() const
    {
        return math_utils::round(pimpl->uiobj->nonClientAreaHeight());
    }

    int Window::contentHeight() const
    {
        return math_utils::round(pimpl->uiobj->clientAreaHeight());
    }

    Panel* Window::content() const
    {
        return pimpl->content;
    }

    void Window::setContent(Panel* uiobj)
    {
        pimpl->content = uiobj;

        if (uiobj != nullptr)
        {
            pimpl->uiobj->setCenterUIObject(uiobj->getImpl()->uiobj);
        }
        else pimpl->uiobj->setCenterUIObject(nullptr);
    }

    bool Window::normal() const
    {
        return pimpl->uiobj->isDisplayNormal();
    }

    bool Window::minimized() const
    {
        return pimpl->uiobj->isDisplayMinimized();
    }

    bool Window::maximized() const
    {
        return pimpl->uiobj->isDisplayMaximized();
    }

    bool Window::minimizeButton() const
    {
        return pimpl->uiobj->isMinimizeEnabled;
    }

    void Window::setMinimizeButton(bool value)
    {
        pimpl->uiobj->isMinimizeEnabled = value;
    }

    bool Window::maximizeButton() const
    {
        return pimpl->uiobj->isMaximizeEnabled;
    }

    void Window::setMaximizeButton(bool value)
    {
        pimpl->uiobj->isMaximizeEnabled = value;
    }

    bool Window::closeButton() const
    {
        return pimpl->uiobj->isCloseEnabled;
    }

    void Window::setCloseButton(bool value)
    {
        pimpl->uiobj->isCloseEnabled = value;
    }

    Window::Callback& Window::callback() const
    {
        return *pcallback;
    }

    void Window::onMinimize() { }

    void Window::onMaximize() { }

    void Window::onRestore() { }

    void Window::onClose() { }
}
