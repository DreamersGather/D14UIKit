#include "Common/Precompile.h"

#include "HorzSlider.h"

#include "Label.h"
#include "Panel.h"
#include "Slider.h"

#include "UIKit/HorzSlider.h"
#include "UIKit/Label.h"

using namespace d14engine;

namespace d14uikit
{
    HorzSlider::HorzSlider() : HorzSlider(Passkey{})
    {
        Panel::pimpl->uiobj =
        HorzSlider::pimpl->uiobj =
        uikit::makeUIObject<uikit::HorzSlider>();

        Panel::initialize();
        HorzSlider::initialize();
    }

    HorzSlider::HorzSlider(Passkey)
        :
        Panel(Panel::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void HorzSlider::initialize()
    {
        pimpl->uiobj->f_onValueChange = [this]
        (uikit::Slider::ValuefulObject* obj, float value)
        {
            onValueChange(value);
            if (pcallback->onValueChange)
            {
                pcallback->onValueChange(this, value);
            }
        };
        pimpl->uiobj->f_onStartSliding = [this]
        (uikit::Slider* sldr, float value)
        {
            onStartSliding(value);
            if (pcallback->onStartSliding)
            {
                pcallback->onStartSliding(this, value);
            }
        };
        pimpl->uiobj->f_onEndSliding = [this]
        (uikit::Slider* sldr, float value)
        {
            onEndSliding(value);
            if (pcallback->onEndSliding)
            {
                pcallback->onEndSliding(this, value);
            }
        };
        // Bind the existing label implementation to the interface.
        {
            pimpl->valueLabel = std::shared_ptr<Label>(new Label(Label::Passkey{}));

            pimpl->valueLabel->Panel::pimpl->uiobj =
            pimpl->valueLabel->Label::pimpl->uiobj =
            pimpl->uiobj->valueLabel();

            pimpl->valueLabel->Panel::initialize();
            pimpl->valueLabel->Label::initialize();
        }
    }

    _D14_UIKIT_SLIDER_IMPL(HorzSlider)

    HorzSlider::Callback& HorzSlider::callback() const { return *pcallback; }

    void HorzSlider::onValueChange(float value) { }

    void HorzSlider::onEndSliding(float value) { }

    void HorzSlider::onStartSliding(float value) { }
}
