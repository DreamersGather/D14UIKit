#include "Common/Precompile.h"

#include "ViewItem.h"

#include "Image.h"
#include "Panel.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/ViewItem.h"

using namespace d14engine;

namespace d14uikit
{
    ViewItem::ViewItem(const std::wstring& text)
        :
        ViewItem(Passkey{})
    {
        Panel::pimpl->uiobj =
        ViewItem::pimpl->uiobj =
        uikit::makeUIObject<uikit::ViewItem>(text);

        Panel::initialize();
        ViewItem::initialize();
    }

    ViewItem::ViewItem(Passkey)
        :
        Panel(Panel::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void ViewItem::initialize()
    {
        setHeight(30);
    }

    Image* ViewItem::icon() const
    {
        return pimpl->icon;
    }

    void ViewItem::setIcon(Image* icon)
    {
        pimpl->icon = icon;

        auto content = pimpl->uiobj->getContent<uikit::IconLabel>().lock();

        if (icon != nullptr && !icon->cpuRead())
        {
            content->icon.bitmap = icon->getImpl()->bitmap;
        }
        else content->icon.bitmap.Reset();

        content->updateLayout();
    }

    Size ViewItem::iconSize() const
    {
        auto& icon = pimpl->uiobj->getContent<uikit::IconLabel>().lock()->icon;

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

    void ViewItem::setIconSize(const Size& value)
    {
        auto content = pimpl->uiobj->getContent<uikit::IconLabel>().lock();

        content->icon.customSize =
        {
            (float)value.width,
            (float)value.height
        };
        content->updateLayout();
    }

    bool ViewItem::customIconSize() const
    {
        auto content = pimpl->uiobj->getContent<uikit::IconLabel>().lock();
        return content->icon.customSize.has_value();
    }

    void ViewItem::setCustomIconSize(bool value)
    {
        auto content = pimpl->uiobj->getContent<uikit::IconLabel>().lock();
        if (value)
        {
            if (!content->icon.customSize.has_value())
            {
                content->icon.customSize = {};
            }
        }
        else content->icon.customSize.reset();

        content->updateLayout();
    }

    const std::wstring& ViewItem::text() const
    {
        return pimpl->uiobj->getContent<uikit::IconLabel>().lock()->label()->text();
    }

    void ViewItem::setText(const std::wstring& text)
    {
        pimpl->uiobj->getContent<uikit::IconLabel>().lock()->label()->setText(text);
    }
}
