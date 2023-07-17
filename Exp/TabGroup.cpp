#include "Common/Precompile.h"

#include "TabGroup.h"

#include "Common.h"
#include "Panel.h"
#include "ResizablePanel.h"
#include "TabCaption.h"

#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/ResourceUtils.h"
#include "UIKit/TabCaption.h"
#include "UIKit/TabGroup.h"
#include "UIKit/Window.h"

using namespace d14engine;

namespace d14uikit
{
    TabGroup::TabGroup()
        :
        TabGroup(Passkey{})
    {
        Panel::pimpl->uiobj =
        ResizablePanel::pimpl->uiobj =
        TabGroup::pimpl->uiobj =
        uikit::makeUIObject<uikit::TabGroup>();

        Panel::initialize();
        ResizablePanel::initialize();
        TabGroup::initialize();
    }

    TabGroup::TabGroup(Passkey)
        :
        Panel(Panel::Passkey{}),
        ResizablePanel(ResizablePanel::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void TabGroup::initialize()
    {
        pimpl->uiobj->f_onTriggerTabPromoting = []
        (uikit::TabGroup* tg, uikit::Window* w)
        {
            w->registerDrawObjects();
            w->registerApplicationEvents();

            w->moveTopmost();

            w->isMinimizeEnabled = false;
            w->isMaximizeEnabled = false;

            w->f_onClose = [](uikit::Window* w) { w->destroy(); };
        };
        pimpl->uiobj->f_onSelectedTabIndexChange = [this]
        (uikit::TabGroup* tg, uikit::TabGroup::TabIndexParam index)
        {
            if (index.valid())
            {
                auto& title = index->caption->title()->label()->text();
                onSelectedChange(title);
                if (pcallback->onSelectedChange)
                {
                    pcallback->onSelectedChange(this, title);
                }
            }
        };
    }

    Size TabGroup::cardSize() const
    {
        auto& appear = pimpl->uiobj->getAppearance().tabBar.card;

        auto index = (size_t)uikit::TabGroup::CardState::Dormant;
        auto& dormantAppear = appear.main[index];

        return convert(dormantAppear.geometry.size);
    }

    void TabGroup::setCardSize(const Size& value)
    {
        auto& barAppear = pimpl->uiobj->getAppearance().tabBar;

        auto& cardAppear = barAppear.card;
        using CState = uikit::TabGroup::CardState;

#define SET_CARD_SIZE(State, W, H) \
    pimpl->uiobj->getAppearance().tabBar.card.main \
    [(size_t)CState::State].geometry.size = { (float)(W), (float)(H) }

            SET_CARD_SIZE(Dormant, value.width,      value.height);
            SET_CARD_SIZE(Hover,   value.width,      value.height);
            SET_CARD_SIZE(Active,  value.width + 16, value.height + 8);

#undef SET_CARD_SIZE

        pimpl->uiobj->loadActiveCardShadowBitmap();

        auto& barGeo = barAppear.geometry;
        auto& sprtGeo = barAppear.separator.geometry;
        auto& prvwCtrlGeo = barAppear.moreCards.control.button.geometry;

        barGeo.height = (float)(value.height + 8);
        sprtGeo.size.height = (float)(value.height - 8);

        auto& geoHeight = prvwCtrlGeo.size.height;
        prvwCtrlGeo.offset.y = ((float)value.height - geoHeight) * 0.5f;

        pimpl->uiobj->updateCandidateTabInfo();
        pimpl->uiobj->updatePreviewPanelItems();
    }

    void TabGroup::insertTab(const Tab& tab, int index)
    {
        pimpl->uiobj->insertTab(
        {
            tab.caption->getImpl()->uiobj,
            tab.content->getImpl()->uiobj
        },
        index);
    }

    void TabGroup::appendTab(const Tab& tab)
    {
        pimpl->uiobj->appendTab(
        {
            tab.caption->getImpl()->uiobj,
            tab.content->getImpl()->uiobj
        });
    }

    void TabGroup::removeTab(int index, int count)
    {
        pimpl->uiobj->removeTab(index, count);
    }

    void TabGroup::clearAllTabs()
    {
        pimpl->uiobj->clearAllTabs();
    }

    int TabGroup::tabCount() const
    {
        return (int)pimpl->uiobj->tabs().size();
    }

    void TabGroup::setCurrSelected(int index)
    {
        pimpl->uiobj->selectTab(index);
    }

    const std::wstring& TabGroup::currSelectedTitle() const
    {
        auto& index = pimpl->uiobj->currActiveCardTabIndex();
        if (index.valid())
        {
            return index->caption->title()->label()->text();
        }
        else return uikit::resource_utils::emptyWstrRef();
    }

    TabGroup::Callback& TabGroup::callback() const { return *pcallback; }

    void TabGroup::onSelectedChange(const std::wstring& title) { }
}
