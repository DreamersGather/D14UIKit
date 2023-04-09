#include "Common/Precompile.h"

#include "TabCaption.h"

#include "Common.h"
#include "Image.h"
#include "Panel.h"
#include "TextFormat.h"

#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/TabCaption.h"

using namespace d14engine;

namespace d14uikit
{
    TabCaption::TabCaption(const std::wstring& title)
        :
        TabCaption(Passkey{})
    {
        Panel::pimpl->uiobj =
        TabCaption::pimpl->uiobj =
        uikit::makeUIObject<uikit::TabCaption>(title);

        Panel::initialize();
        TabCaption::initialize();
    }

    TabCaption::TabCaption(Passkey)
        :
        Panel(Panel::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void TabCaption::initialize() { }

    Image* TabCaption::icon() const
    {
        return pimpl->icon;
    }

    void TabCaption::setIcon(Image* icon)
    {
        pimpl->icon = icon;
        auto& targetIcon = pimpl->uiobj->title()->icon;
        if (icon != nullptr && !icon->cpuRead())
        {
            targetIcon.bitmap = icon->getImpl()->bitmap;
        }
        else targetIcon.bitmap.Reset();

        pimpl->uiobj->title()->updateLayout();
    }

    Size TabCaption::iconSize() const
    {
        auto& icon = pimpl->uiobj->title()->icon;

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

    void TabCaption::setIconSize(const std::optional<Size>& value)
    {
        auto& icon = pimpl->uiobj->title()->icon;

        if (value.has_value())
        {
            icon.customSize = convert(value.value());
        }
        else icon.customSize.reset();

        pimpl->uiobj->title()->updateLayout();
    }

    const std::wstring& TabCaption::title() const
    {
        return pimpl->uiobj->title()->label()->text();
    }

    void TabCaption::setTitle(const std::wstring& title)
    {
        auto& icolbl = pimpl->uiobj->title();
        icolbl->label()->setText(title);
        icolbl->updateLayout();
    }

    _D14_UIKIT_TEXT_FORMAT_IMPL_CONCRETE(
        TabCaption, , label, pimpl->uiobj->title())

    bool TabCaption::closable() const
    {
        return pimpl->uiobj->closable;
    }

    void TabCaption::setClosable(bool value)
    {
        pimpl->uiobj->closable = value;
    }

    bool TabCaption::draggable() const
    {
        return pimpl->uiobj->draggable;
    }

    void TabCaption::setDraggable(bool value)
    {
        pimpl->uiobj->draggable = value;
    }

    bool TabCaption::promotable() const
    {
        return pimpl->uiobj->promotable;
    }

    void TabCaption::setPromotable(bool value)
    {
        pimpl->uiobj->promotable = value;
    }
}
