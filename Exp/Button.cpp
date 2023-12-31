﻿#include "Common/Precompile.h"

#include "Button.h"

#include "ClickablePanel.h"
#include "Common.h"
#include "Image.h"
#include "Panel.h"
#include "TextFormat.h"

#include "UIKit/Button.h"
#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"

using namespace d14engine;

namespace d14uikit
{
    Button::Button(const std::wstring& text)
        :
        Button(Passkey{})
    {
        Panel::pimpl->uiobj =
        ClickablePanel::pimpl->uiobj =
        Button::pimpl->uiobj =
        uikit::makeUIObject<uikit::Button>(text);

        Panel::initialize();
        ClickablePanel::initialize();
        Button::initialize();
    }

    Button::Button(Passkey)
        :
        Panel(Panel::Passkey{}),
        ClickablePanel(ClickablePanel::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void Button::initialize() { }

    Image* Button::icon() const
    {
        return pimpl->icon;
    }

    void Button::setIcon(Image* icon)
    {
        pimpl->icon = icon;
        auto& targetIcon = pimpl->uiobj->content()->icon;
        if (icon != nullptr && !icon->cpuRead())
        {
            targetIcon.bitmap = icon->getImpl()->bitmap;
        }
        else targetIcon.bitmap.Reset();

        pimpl->uiobj->content()->updateLayout();
    }

    Size Button::iconSize() const
    {
        auto& icon = pimpl->uiobj->content()->icon;

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

    void Button::setIconSize(const std::optional<Size>& value)
    {
        auto& icon = pimpl->uiobj->content()->icon;

        if (value.has_value())
        {
            icon.customSize = convert(value.value());
        }
        else icon.customSize.reset();

        pimpl->uiobj->content()->updateLayout();
    }

    const std::wstring& Button::text() const
    {
        return pimpl->uiobj->content()->label()->text();
    }

    void Button::setText(const std::wstring& text)
    {
        pimpl->uiobj->content()->label()->setText(text);

        pimpl->uiobj->content()->updateLayout();
    }

    _D14_UIKIT_TEXT_FORMAT_IMPL_CONCRETE(
        Button, , label, pimpl->uiobj->content())
}
