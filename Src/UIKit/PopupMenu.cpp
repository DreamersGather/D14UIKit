#include "Common/Precompile.h"

#include "UIKit/PopupMenu.h"

#include "Common/CppLangUtils/PointerEquality.h"
#include "UIKit/Application.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    PopupMenu::PopupMenu(const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        WaterfallView(rect),
        shadow(math_utils::roundu(extendedSize(size())))
    {
        setVisible(false);
        setEnabled(false);

        selectMode = SelectMode::None;
    }

    D2D1_SIZE_F PopupMenu::extendedSize(const D2D1_SIZE_F& rawSize) const
    {
        return math_utils::increaseHeight(rawSize, +2.0f * getAppearance().geometry.extension);
    }

    D2D1_SIZE_F PopupMenu::narrowedSize(const D2D1_SIZE_F& rawSize) const
    {
        return math_utils::increaseHeight(rawSize, -2.0f * getAppearance().geometry.extension);
    }

    void PopupMenu::onChangeActivity(bool value)
    {
        onChangeActivityHelper(value);

        if (f_onChangeActivity) f_onChangeActivity(this, value);
    }

    void PopupMenu::onTriggerMenuItem(ItemIndexParam itemIndex)
    {
        onTriggerMenuItemHelper(itemIndex);

        if (f_onTriggerMenuItem) f_onTriggerMenuItem(this, itemIndex);
    }

    void PopupMenu::onChangeActivityHelper(bool value)
    {
        Application::g_app->sendNextImmediateMouseMoveEvent = true;
    }

    void PopupMenu::onTriggerMenuItemHelper(ItemIndexParam itemIndex)
    {
        // This method intentionally left blank.
    }

    void PopupMenu::insertItem(const ItemList& items, size_t index)
    {
        for (auto& item : items)
        {
            item->m_parentMenu = std::dynamic_pointer_cast<PopupMenu>(shared_from_this());
        }
        WaterfallView::insertItem(items, index);
    }

    void PopupMenu::appendItem(const ItemList& items)
    {
        insertItem(items, m_items.size());
    }

    void PopupMenu::removeItem(size_t index, size_t count)
    {
        if (index >= 0 && index < m_items.size() && count > 0)
        {
            count = std::min(count, m_items.size() - index);
            size_t endIndex = index + count - 1;

            for (ItemIndex itemIndex = { &m_items, index }; itemIndex < endIndex; ++itemIndex)
            {
                (*itemIndex)->m_parentMenu.reset();
            }
        }
        WaterfallView::removeItem(index, count);
    }

    const WeakPtr<MenuItem>& PopupMenu::associatedItem() const
    {
        return m_associatedItem;
    }

    void PopupMenu::setActivated(bool value)
    {
        setVisible(value);
        setEnabled(value);

        if (m_backgroundTriggerPanel)
        {
            m_backgroundTriggerPanel->setEnabled(value);
            if (value) m_backgroundTriggerPanel->moveTopmost();;
        }
        if (value)
        {
            // Place the menu above the bkgn-trigger-panel.
            moveTopmost();
        }
        else if (m_lastHoverItemIndex.valid())
        {
            (*m_lastHoverItemIndex)->triggerLeaveStateTrans();
            m_lastHoverItemIndex.invalidate();
        }
        onChangeActivity(value);
    }

    void PopupMenu::setActivatedIncludingParents(bool value)
    {
        setActivated(value);

        if (!m_associatedItem.expired() && !m_associatedItem.lock()->m_parentMenu.expired())
        {
            m_associatedItem.lock()->m_parentMenu.lock()->setActivatedIncludingParents(value);
        }
    }

    void PopupMenu::setActivatedIncludingChildren(bool value)
    {
        if (m_lastHoverItemIndex.valid() && (*m_lastHoverItemIndex)->m_associatedMenu != nullptr)
        {
            (*m_lastHoverItemIndex)->m_associatedMenu->setActivatedIncludingChildren(value);
        }
        setActivated(value); // Note m_lastHoverItemIndex may be invalidated in setActivated.
    }

    bool PopupMenu::isHorzConstraintMeet(float expectedHorzOffset)
    {
        return expectedHorzOffset + extendedSize(size()).width <= constrainedRectangle.right;
    }

    bool PopupMenu::isVertConstraintMeet(float expectedVertOffset)
    {
        return expectedVertOffset + extendedSize(size()).height <= constrainedRectangle.bottom;
    }

    void PopupMenu::showInConstrainedRect(D2D1_POINT_2F expectedOffset)
    {
        auto extSize = extendedSize(size());

        if (!isHorzConstraintMeet(expectedOffset.x))
        {
            expectedOffset.x = constrainedRectangle.right - extSize.width;
        }
        expectedOffset.x = std::max(expectedOffset.x, constrainedRectangle.left);

        if (!isVertConstraintMeet(expectedOffset.y))
        {
            expectedOffset.y = std::min(expectedOffset.y, constrainedRectangle.bottom) - extSize.height;
        }
        expectedOffset.y = std::max(expectedOffset.y, constrainedRectangle.top);

        move(math_utils::increaseY(expectedOffset, getAppearance().geometry.extension));
        setActivated(true); // Note a vast menu could still overflow in both directions.
    }

    WeakPtr<Panel> PopupMenu::backgroundTriggerPanel() const
    {
        return m_backgroundTriggerPanel;
    }

    void PopupMenu::setBackgroundTriggerPanel(bool value)
    {
        if (value)
        {
            m_backgroundTriggerPanel = makeRootUIObject<Panel>(math_utils::infiniteRectFRef());

            m_backgroundTriggerPanel->setVisible(false);
            m_backgroundTriggerPanel->setEnabled(false);

            m_backgroundTriggerPanel->f_onMouseButton = [this](Panel* p, MouseButtonEvent& e)
            {
                setActivatedIncludingChildren(false);
            };
        }
        else if (m_backgroundTriggerPanel && m_backgroundTriggerPanel->destroy())
        {
            m_backgroundTriggerPanel.reset();
        }
    }

    void PopupMenu::onRendererDrawD2d1LayerHelper(Renderer* rndr)
    {
        WaterfallView::onRendererDrawD2d1LayerHelper(rndr);

        // Shape of Shadow
        shadow.beginShadowDraw(rndr->d2d1DeviceContext());
        {
            resource_utils::g_solidColorBrush->SetOpacity(1.0f);

            auto extendedRect = math_utils::rect({ 0.0f, 0.0f }, extendedSize(size()));

            rndr->d2d1DeviceContext()->FillRoundedRectangle(
            {
                math_utils::moveVertex(extendedRect, getAppearance().shadow.offset),
                getAppearance().geometry.roundRadius, getAppearance().geometry.roundRadius
            },
            resource_utils::g_solidColorBrush.Get());
        }
        shadow.endShadowDraw(rndr->d2d1DeviceContext());
    }

    void PopupMenu::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        // Shadow
        auto& geoSetting = getAppearance().geometry;
        auto& shadowSetting = getAppearance().shadow;

        shadow.color = shadowSetting.color;
        shadow.standardDeviation = shadowSetting.standardDeviation;

        shadow.configShadowEffectInput(resource_utils::g_shadowEffect.Get());

        auto leftTop = absolutePosition();
        auto shadowLeftTop = math_utils::increaseY(leftTop, -geoSetting.extension);

        rndr->d2d1DeviceContext()->DrawImage(
            resource_utils::g_shadowEffect.Get(), math_utils::roundf(shadowLeftTop));

        // Extension
        auto& extBkgn = getAppearance().background;

        resource_utils::g_solidColorBrush->SetColor(extBkgn.color);
        resource_utils::g_solidColorBrush->SetOpacity(extBkgn.opacity);

        rndr->d2d1DeviceContext()->FillRoundedRectangle(
        {
            math_utils::stretch(m_absoluteRect, { 0.0f, geoSetting.extension }),
            geoSetting.roundRadius, geoSetting.roundRadius
        },
        resource_utils::g_solidColorBrush.Get());

        // Body Content
        WaterfallView::onRendererDrawD2d1ObjectHelper(rndr);
    }

    void PopupMenu::onSizeHelper(SizeEvent& e)
    {
        WaterfallView::onSizeHelper(e);

        shadow.loadShadowBitmap(math_utils::roundu(extendedSize(e.size)));
    }

    void PopupMenu::onChangeThemeHelper(WstrParam themeName)
    {
        WaterfallView::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(themeName);
    }

    void PopupMenu::onMouseMoveHelper(MouseMoveEvent& e)
    {
        ItemIndex lastHoverItemIndex = m_lastHoverItemIndex;

        WaterfallView::onMouseMoveHelper(e);

        ItemIndex& currHoverItemIndex = m_lastHoverItemIndex;

        if (lastHoverItemIndex.valid() && lastHoverItemIndex != currHoverItemIndex)
        {
            if ((*lastHoverItemIndex)->m_associatedMenu != nullptr)
            {
                (*lastHoverItemIndex)->m_associatedMenu->setActivated(false);
            }
        }
        if (currHoverItemIndex.valid() && currHoverItemIndex != lastHoverItemIndex)
        {
            if ((*currHoverItemIndex)->m_associatedMenu != nullptr)
            {
                (*currHoverItemIndex)->popupAssociatedMenu();
            }
        }
    }

    void PopupMenu::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        ScrollView::onMouseLeaveHelper(e);

        if (m_lastHoverItemIndex.valid())
        {
            if ((*m_lastHoverItemIndex)->m_associatedMenu == nullptr)
            {
                (*m_lastHoverItemIndex)->triggerLeaveStateTrans();
                m_lastHoverItemIndex.invalidate();
            }
        }
    }

    void PopupMenu::onMouseButtonHelper(MouseButtonEvent& e)
    {
        WaterfallView::onMouseButtonHelper(e);

        // In case trigger by mistake when controlling the scroll bars.
        if (e.state.leftUp() && !isControllingScrollBars())
        {
            auto itemIndex = viewportOffsetToItemIndex(
                m_viewportOffset.y + absoluteToSelfCoord(e.cursorPoint).y);

            if (itemIndex.valid() && (*itemIndex)->m_enabled)
            {
                onTriggerMenuItem(itemIndex);
            }
        }
    }
}
