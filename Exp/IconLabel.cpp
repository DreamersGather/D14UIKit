#include "Common/Precompile.h"

#include "IconLabel.h"

#include "Common.h"
#include "Image.h"
#include "Label.h"

#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"

using namespace d14engine;

namespace d14uikit
{
    IconLabel::IconLabel(const std::wstring& text)
        :
        IconLabel(uikit::makeUIObject<uikit::IconLabel>(text)) { }

    _D14_UIKIT_CTOR(IconLabel)
        :
        Panel(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;

        _D14_UIKIT_BIND(Label, label);
    }

    Image* IconLabel::icon() const
    {
        return pimpl->icon;
    }

    void IconLabel::setIcon(Image* icon)
    {
        pimpl->icon = icon;

        auto& targetIcon = pimpl->uiobj->icon;
        if (icon != nullptr && !icon->cpuRead())
        {
            targetIcon.bitmap = icon->getImpl()->bitmap;
        }
        else targetIcon.bitmap.Reset();

        pimpl->uiobj->updateLayout();
    }

    Size IconLabel::iconSize() const
    {
        auto& icon = pimpl->uiobj->icon;

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

    void IconLabel::setIconSize(const std::optional<Size>& value)
    {
        auto& icon = pimpl->uiobj->icon;

        if (value.has_value())
        {
            icon.customSize = convert(value.value());
        }
        else icon.customSize.reset();

        pimpl->uiobj->updateLayout();
    }

    Label* IconLabel::label() const
    {
        return pimpl->label.get();
    }

    void IconLabel::updateLayout()
    {
        pimpl->uiobj->updateLayout();
    }
}
