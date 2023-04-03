#include "Common/Precompile.h"

#include "Button.h"

#include "ClickablePanel.h"
#include "Image.h"
#include "Panel.h"

#include "Common/MathUtils/Basic.h"

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
        return
        {
            math_utils::round(iconSize.width),
            math_utils::round(iconSize.height)
        };
    }

    void Button::setIconSize(const Size& value)
    {
        pimpl->uiobj->content()->icon.customSize =
        {
            (float)value.width,
            (float)value.height
        };
        pimpl->uiobj->content()->updateLayout();
    }

    bool Button::customIconSize() const
    {
        return pimpl->uiobj->content()->icon.customSize.has_value();
    }

    void Button::setCustomIconSize(bool value)
    {
        auto& icon = pimpl->uiobj->content()->icon;

        if (!value) icon.customSize.reset();
        else if (!icon.customSize.has_value())
        {
            icon.customSize = {};
        }
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
}
