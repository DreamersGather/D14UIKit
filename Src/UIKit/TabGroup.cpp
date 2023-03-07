#include "Common/Precompile.h"

#include "UIKit/TabGroup.h"

#include "Common/CppLangUtils/PointerEquality.h"
#include "Common/DirectXError.h"
#include "UIKit/Application.h"
#include "UIKit/Cursor.h"
#include "UIKit/IconLabel.h"
#include "UIKit/PopupMenu.h"
#include "UIKit/ResourceUtils.h"
#include "UIKit/TabCaption.h"
#include "UIKit/Window.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    TabGroup::TabGroup(const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        ResizablePanel(rect, resource_utils::g_solidColorBrush)
    {
        m_takeOverChildrenDrawing = true;

        setResizable(false);

        transform(math_utils::adaptMaxSize(rect, minimalSize()));

        m_previewPanel = makeRootUIObject<PopupMenu>();
        m_previewPanel->setBackgroundTriggerPanel(true);
    }

    TabGroup::~TabGroup()
    {
        m_previewPanel->destroy();
    }

    void TabGroup::onInitializeFinish()
    {
        ResizablePanel::onInitializeFinish();

        loadActiveCardShadowBitmap();
    }

    void TabGroup::loadActiveCardShadowBitmap()
    {
        auto& setting = getAppearance().tabBar.card.main[(size_t)CardState::Active];

        activeCardShadow.loadShadowBitmap(math_utils::roundu(setting.geometry.size));
    }

    D2D1_RECT_F TabGroup::cardBarExtendedAbsoluteRect() const
    {
        return math_utils::increaseTop(m_absoluteRect, -getAppearance().tabBar.geometry.height);
    }

    D2D1_RECT_F TabGroup::cardBarExtendedCardBarAbsoluteRect() const
    {
        auto& setting = getAppearance().tabBar.geometry;
        
        return math_utils::rect(m_absoluteRect.left, m_absoluteRect.top - setting.height, width(), setting.height);
    }

    TabGroup::TabIndex TabGroup::selfcoordOffsetToCardTabIndex(float offset) const
    {
        float cardLength = 0.0f;
        for (TabIndex tabIndex = { (TabList*)&m_tabs, 0 }; tabIndex < m_candidateTabCount; ++tabIndex)
        {
            auto state = getCardState(tabIndex);
            auto& setting = getAppearance().tabBar.card.main[(size_t)state];

            cardLength += setting.geometry.size.width;
            if (cardLength > offset) return tabIndex;
        }
        return TabIndex{};
    }

    void TabGroup::onSelectedTabIndexChange(TabIndexParam index)
    {
        onSelectedTabIndexChangeHelper(index);

        if (f_onSelectedTabIndexChange) f_onSelectedTabIndexChange(this, index);
    }

    void TabGroup::onSelectedTabIndexChangeHelper(TabIndexParam index)
    {
        // This method intentionally left blank.
    }

    const TabGroup::TabList& TabGroup::tabs() const
    {
        return m_tabs;
    }

    void TabGroup::insertTab(const Tab& tab, size_t index)
    {
        if (!tab.caption || !tab.content) return;

        // "index == m_tabs.size()" ---> append
        index = std::clamp(index, 0ull, m_tabs.size());

        insertTab(tab, { &m_tabs, index });
    }

    void TabGroup::appendTab(const Tab& tab)
    {
        insertTab(tab, m_tabs.size());
    }

    void TabGroup::removeTab(size_t index, size_t count)
    {
        if (index >= 0 && index < m_tabs.size() && count > 0)
        {
            count = std::min(count, m_tabs.size() - index);

            removeTab({ &m_tabs, index }, count);
        }
    }

    void TabGroup::clearAllTabs()
    {
        // There is no trivial clearing since we need to update each tab-caption.
        removeTab(0, m_tabs.size());
    }

    void TabGroup::selectTab(size_t index)
    {
        if (index >= 0 && index < m_tabs.size())
        {
            selectTab({ &m_tabs, index });
        }
        else if (m_currActiveCardTabIndex.valid())
        {
            m_currActiveCardTabIndex->content->setEnabled(false);
            m_currActiveCardTabIndex.invalidate();

            updateCandidateTabInfo();
            updatePreviewPanelItems();
        }
    }

    void TabGroup::swapTab(size_t index1, size_t index2)
    {
        if (index1 >= 0 && index1 < m_tabs.size() &&
            index2 >= 0 && index2 < m_tabs.size() && index1 != index2)
        {
            TabIndex tabIndex1, tabIndex2;
            for (TabIndex tabIndex = { &m_tabs, 0 }; tabIndex.valid(); ++tabIndex)
            {
                if (tabIndex.index == index1) tabIndex1 = tabIndex;
                if (tabIndex.index == index2) tabIndex2 = tabIndex;

                if (tabIndex1.valid() && tabIndex2.valid()) break; // both found
            }
            swapTab(tabIndex1, tabIndex2);
        }
    }

    void TabGroup::insertTab(const Tab& tab, TabIndexParam tabIndex)
    {
        if (!tab.caption || !tab.content) return;

        addUIObject(tab.content);

        tab.caption->m_parentTabGroup = std::dynamic_pointer_cast<TabGroup>(shared_from_this());

        tab.content->skipDrawPosteriorObjects = true;
        tab.content->setEnabled(false);
        tab.content->transform(selfCoordRect());

        auto tabItor = m_tabs.insert(tabIndex.iterator, tab);

        tabItor->m_previewItem = makeUIObject<MenuItem>();
        tabItor->m_previewItem->isInstant = false;

#define UPDATE_TAB_INDEX(Tab_Index) \
do { \
    if (Tab_Index.generalValid()) \
    { \
        if (insertedIndex <= Tab_Index) \
        { \
            Tab_Index.moveIndexNext(); \
        } \
    } \
} while (0)
        // Note tabIndex may be invalidated if it is one of the following.
        size_t insertedIndex = tabIndex.index;

        UPDATE_TAB_INDEX(m_currHoverCardTabIndex);
        UPDATE_TAB_INDEX(m_currActiveCardTabIndex);
        UPDATE_TAB_INDEX(m_currDraggedCardTabIndex);

#undef UPDATE_TAB_INDEX

        updateCandidateTabInfo();
        if (tabIndex < m_candidateTabCount)
        {
            addUIObject(tabItor->caption);
        }
        else // not a candidate tab
        {
            tabItor->m_previewItem->setContent(tabItor->caption);
        }
        updatePreviewPanelItems();
    }

    void TabGroup::removeTab(TabIndexParam tabIndex, size_t count)
    {
        size_t orgActiveCardIndex = m_currActiveCardTabIndex.index;

        TabList::iterator tabItor = tabIndex.iterator;

        for (size_t i = 0; i < count; ++i)
        {
            tabItor->caption->destroy();
            removeUIObject(tabItor->content);
            tabItor->m_previewItem->destroy();

            tabItor->caption->m_parentTabGroup.reset();

            tabItor = m_tabs.erase(tabItor);
        }
        size_t endIndex = tabIndex.index + count;

#define UPDATE_TAB_INDEX(Tab_Index) \
do { \
    if (Tab_Index.generalValid()) \
    { \
        if (Tab_Index >= endIndex) \
        { \
            Tab_Index.moveIndexPrev(count); \
        } \
        else if (Tab_Index >= removedIndex) \
        { \
            Tab_Index.invalidate(); \
        } \
    } \
} while (0)
        // Note tabIndex may be invalidated if it is one of the following.
        size_t removedIndex = tabIndex.index;

        UPDATE_TAB_INDEX(m_currHoverCardTabIndex);
        UPDATE_TAB_INDEX(m_currActiveCardTabIndex);
        UPDATE_TAB_INDEX(m_currDraggedCardTabIndex);

#undef UPDATE_TAB_INDEX

        updateCandidateTabInfo();

        if (m_currActiveCardTabIndex != orgActiveCardIndex)
        {
            onSelectedTabIndexChange(m_currActiveCardTabIndex);
        }
        updatePreviewPanelItems();
    }

    void TabGroup::selectTab(TabIndexParam tabIndex)
    {
        size_t orgActiveCardIndex = m_currActiveCardTabIndex.index;

        if (tabIndex != m_currActiveCardTabIndex)
        {
            if (m_currActiveCardTabIndex.valid())
            {
                m_currActiveCardTabIndex->content->setEnabled(false);
            }
        }
        if (tabIndex >= m_candidateTabCount)
        {
            if (!m_currActiveCardTabIndex.valid())
            {
                m_currActiveCardTabIndex = TabIndex::begin(&m_tabs);
            }
            swapTab(tabIndex, m_currActiveCardTabIndex);
        }
        else m_currActiveCardTabIndex = tabIndex;

        updateCandidateTabInfo();

        if (m_candidateTabCount > 0)
        {
            m_currActiveCardTabIndex->content->setEnabled(true);
        }
        else m_currActiveCardTabIndex.invalidate();

        if (m_currActiveCardTabIndex.valid())
        {
            m_currActiveCardTabIndex->content->transform(selfCoordRect());
        }
        if (m_currActiveCardTabIndex != orgActiveCardIndex)
        {
            onSelectedTabIndexChange(m_currActiveCardTabIndex);
        }
        updatePreviewPanelItems();
    }

    void TabGroup::swapTab(TabIndexParam tabIndex1, TabIndexParam tabIndex2)
    {
        std::swap(*tabIndex1.iterator, *tabIndex2.iterator);
    }

    TabGroup::CardState TabGroup::getCardState(TabIndexParam tabIndex) const
    {
        if (tabIndex.valid())
        {
            if (tabIndex == m_currActiveCardTabIndex)
            {
                return CardState::Active;
            }
            else if (tabIndex == m_currHoverCardTabIndex)
            {
                return CardState::Hover;
            }
            // fall through
        }
        return CardState::Dormant;
    }

    const TabGroup::TabIndex& TabGroup::currActiveCardTabIndex() const
    {
        return m_currActiveCardTabIndex;
    }

    TabGroup::ButtonState TabGroup::getMoreCardsButtonState() const
    {
        if (m_isMoreCardsButtonDown)
        {
            return ButtonState::Down;
        }
        else if (m_isMoreCardsButtonHover)
        {
            return ButtonState::Hover;
        }
        else return ButtonState::Idle;
    }

    void TabGroup::updatePreviewPanelItems()
    {
        D2D1_POINT_2F orgViewportOffset = m_previewPanel->viewportOffset();

        TabIndex tabIndex = { &m_tabs, 0 };
        for ( ; tabIndex < m_candidateTabCount; ++tabIndex )
        {
            tabIndex->caption->destroy();
            addUIObject(tabIndex->caption);
        }
        m_previewPanel->clearAllItems();

        auto& cardSetting = getAppearance().tabBar.card;
        auto& prvwSrc = getAppearance().tabBar.moreCards.previewPanel;

        auto& prvwDst = m_previewPanel->getAppearance();
        // LoadShadowBitmap will be called when resizing the preview-panel.
        prvwDst.geometry = prvwSrc.geometry;
        prvwDst.shadow.offset = prvwSrc.shadow.offset;
        prvwDst.shadow.standardDeviation = prvwSrc.shadow.standardDeviation;

        PopupMenu::ItemList prvwItems = {};

        // Insert the non-candidate tabs into the preview-panel.
        for ( ; tabIndex.valid(); ++tabIndex )
        {
            tabIndex->m_previewItem->transform(math_utils::heightOnlyRect(prvwSrc.itemHeight));
            tabIndex->caption->destroy();
            tabIndex->m_previewItem->setContent(tabIndex->caption);
            tabIndex->m_previewItem->state = ViewItem::State::Idle;

            prvwItems.push_back(tabIndex->m_previewItem);
        }
        m_previewPanel->insertItem(prvwItems);

        float prvwWidth = cardSetting.main[(size_t)CardState::Dormant].geometry.size.width;
        float prvwHeight = (float)(m_tabs.size() - m_candidateTabCount) * prvwSrc.itemHeight;

        // Keep the preview-panel is within the area of the tab-group.
        auto prvwSize = m_previewPanel->extendedSize({ prvwWidth, prvwHeight });
        if (prvwSize.height > height())
        {
            m_previewPanel->resize(m_previewPanel->narrowedSize({ prvwSize.width, height() }));
        }
        else m_previewPanel->resize({ prvwWidth, prvwHeight });

        m_previewPanel->move(math_utils::offset(
            math_utils::rightTop(m_absoluteRect),
            math_utils::increaseX(prvwSrc.offset, -m_previewPanel->width())));

        m_previewPanel->setViewportOffset(orgViewportOffset);
    }

    const SharedPtr<PopupMenu>& TabGroup::previewPanel() const
    {
        return m_previewPanel;
    }

    void TabGroup::updateCandidateTabInfo()
    {
        TabIndex orgIndex = { &m_tabs, 0 };
        for (; orgIndex < m_candidateTabCount && orgIndex.valid(); ++orgIndex)
        {
            orgIndex->caption->destroy();
            orgIndex->m_previewItem->setContent(orgIndex->caption);
        }
        m_candidateTabCount = m_tabs.size();

        float cardLength = 0.0f;
        float maxCardLegnth = width() - getAppearance().tabBar.geometry.rightPadding;

        for (TabIndex tabIndex = { &m_tabs, 0 }; tabIndex.valid(); ++tabIndex)
        {
            auto state = getCardState(tabIndex);
            auto& setting = getAppearance().tabBar.card.main[(size_t)state];

            float tmpLength = cardLength + setting.geometry.size.width;
            if (tmpLength > maxCardLegnth)
            {
                m_candidateTabCount = tabIndex.index;
                break;
            }
            tabIndex->m_cardAbsoluteRectCache =
            {
                m_absoluteRect.left + cardLength,
                m_absoluteRect.top - setting.geometry.size.height,
                m_absoluteRect.left + cardLength + setting.geometry.size.width,
                m_absoluteRect.top + ((state == CardState::Active) ? 0.0f : setting.geometry.roundRadius)
            };
            cardLength = tmpLength;

            tabIndex->caption->destroy();
            addUIObject(tabIndex->caption);

            tabIndex->caption->transform(absoluteToSelfCoord(cardCaptionAbsoluteRect(tabIndex)));
        }
    }

    D2D1_RECT_F TabGroup::cardBarAbsoluteRect() const
    {
        auto& setting = getAppearance().tabBar.card.main[(size_t)CardState::Dormant];
        return
        {
            m_absoluteRect.left,
            m_absoluteRect.top - setting.geometry.size.height,
            m_absoluteRect.right,
            m_absoluteRect.top + setting.geometry.roundRadius
        };
    }

    const D2D1_RECT_F& TabGroup::cardAbsoluteRect(TabIndexParam tabIndex) const
    {
        return tabIndex.valid() ? tabIndex->m_cardAbsoluteRectCache : math_utils::zeroRectFRef();
    }

    D2D1_RECT_F TabGroup::cardCaptionAbsoluteRect(TabIndexParam tabIndex) const
    {
        auto state = getCardState(tabIndex);
        float roundRaidus = getAppearance().tabBar.card.main[(size_t)state].geometry.roundRadius;

        if (state == CardState::Active) // Clip margins.
        {
            return math_utils::stretch(cardAbsoluteRect(tabIndex), { -roundRaidus, 0.0f });
        }
        else // Dormant, Hover: Crop only the upper half.
        {
            return math_utils::increaseBottom(cardAbsoluteRect(tabIndex), -roundRaidus);
        }
    }

    D2D1_RECT_F TabGroup::separatorAbsoluteRect(TabIndexParam tabIndex) const
    {
        auto& setting = getAppearance().tabBar.separator.geometry;

        return math_utils::rect(math_utils::offset(math_utils::rightTop(cardAbsoluteRect(tabIndex)), setting.offset), setting.size);
    }

    D2D1_RECT_F TabGroup::moreCardsIconAbsoluteRect() const
    {
        auto& setting = getAppearance().tabBar.moreCards.control.icon.geometry.topRect;

        return math_utils::rect(math_utils::offset(math_utils::leftTop(moreCardsButtonAbsoluteRect()), setting.offset), setting.size);
    }

    math_utils::Triangle2D TabGroup::moreCardsIconAbsoluteTriangle() const
    {
        auto relative = math_utils::leftTop(moreCardsButtonAbsoluteRect());
        auto& setting = getAppearance().tabBar.moreCards.control.icon.geometry.bottomTriangle;
        return
        {
            math_utils::offset(relative, setting.point0),
            math_utils::offset(relative, setting.point1),
            math_utils::offset(relative, setting.point2)
        };
    }

    D2D1_RECT_F TabGroup::moreCardsButtonAbsoluteRect() const
    {
        auto& setting = getAppearance().tabBar.moreCards.control.button.geometry;

        return math_utils::rect(math_utils::offset(math_utils::rightTop(cardBarAbsoluteRect()), setting.offset), setting.size);
    }

    SharedPtr<Window> TabGroup::promoteTabToWindow(size_t index)
    {
        if (index >= 0 && index < m_tabs.size())
        {
            return promoteTabToWindow({ &m_tabs, index });
        }
        else return nullptr;
    }

    SharedPtr<Window> TabGroup::promoteTabToWindow(TabIndexParam tabIndex)
    {
        auto rect = math_utils::increaseTop
        (
            tabIndex->content->absoluteRect(), -Window::nonClientAreaDefaultHeight()
        );
        auto w = makeUIObject<Window>(tabIndex->caption->title(), rect);

        tabIndex->content->skipDrawPosteriorObjects = false;
        tabIndex->content->setEnabled(true);

        w->setCenterUIObject(tabIndex->content);

        removeTab(tabIndex, 1);

        w->minimalWidthHint = std::max(minimalWidth(), Window::nonClientAreaMinimalWidth());
        w->minimalHeightHint = minimalHeight() + Window::nonClientAreaDefaultHeight();

        w->maximalWidthHint = std::max(maximalWidth(), Window::nonClientAreaMinimalWidth());
        w->maximalHeightHint = maximalHeight() + Window::nonClientAreaDefaultHeight();

        return w;
    }

    void TabGroup::triggerTabPromoting(MouseMoveEvent& e)
    {
        if (m_currDraggedCardTabIndex.valid() && m_currDraggedCardTabIndex->caption->promotable)
        {
            auto w = promoteTabToWindow(m_currDraggedCardTabIndex);

            // Ensure the cursor is within the caption of the promoted window,
            // so we can send a pseudo mouse-button event to trigger dragging.

            D2D1_POINT_2F offset =
            {
                -Window::nonClientAreaMinimalWidth() * 0.5f,
                -w->nonClientAreaHeight() * 0.5f
            };
            w->move(math_utils::offset(e.cursorPoint, offset));

            Application::g_app->focusUIObject(w);

            MouseButtonEvent immediateMouseButton = {};
            immediateMouseButton.cursorPoint = e.cursorPoint;
            immediateMouseButton.state = { MouseButtonEvent::State::Flag::LeftDown };

            w->onMouseButton(immediateMouseButton);

            w->registerTabGroup(std::dynamic_pointer_cast<TabGroup>(shared_from_this()));

            if (f_onTriggerTabPromoting) f_onTriggerTabPromoting(this, w.get());
        }
    }

    bool TabGroup::isAssociatedWindowDraggedAbove() const
    {
        if (!associatedWindow.expired())
        {
            auto targetWindow = associatedWindow.lock();
            auto targetTabGroup = targetWindow->associatedTabGroup.lock();

            return cpp_lang_utils::isMostDerivedEqual(targetTabGroup, shared_from_this());
        }
        else return false;
    }

    void TabGroup::onRendererDrawD2d1LayerHelper(Renderer* rndr)
    {
        if (m_currActiveCardTabIndex.valid())
        {
            if (m_currActiveCardTabIndex->content->isD2d1ObjectVisible())
            {
                m_currActiveCardTabIndex->content->onRendererDrawD2d1Layer(rndr);
            }
            // Active-Card Shadow
            activeCardShadow.beginShadowDraw(rndr->d2d1DeviceContext());
            {
                auto& setting = getAppearance().tabBar.card.main[(size_t)CardState::Active];

                float cardWidth = setting.geometry.size.width;
                float cardHeight = setting.geometry.size.height;

                float cardRoundRadius = setting.geometry.roundRadius;
                D2D1_SIZE_F cardCornerSize = { cardRoundRadius, cardRoundRadius };

                ComPtr<ID2D1PathGeometry> pathGeo;
                THROW_IF_FAILED(rndr->d2d1Factory()->CreatePathGeometry(&pathGeo));

                ComPtr<ID2D1GeometrySink> geoSink;
                THROW_IF_FAILED(pathGeo->Open(&geoSink));
                {
                    geoSink->BeginFigure({ 0.0f, cardHeight }, D2D1_FIGURE_BEGIN_FILLED);

                    // Left Bottom Corner
                    geoSink->AddArc(
                    {
                        /* point            */ { cardRoundRadius, cardHeight - cardRoundRadius },
                        /* size             */ cardCornerSize,
                        /* rotation degrees */ 90.0f,
                        /* sweep direction  */ D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
                        /* arc size         */ D2D1_ARC_SIZE_SMALL
                    });
                    geoSink->AddLine({ cardRoundRadius, cardRoundRadius });

                    // Left Top Corner
                    geoSink->AddArc(
                    {
                        /* point            */ { cardRoundRadius * 2.0f, 0.0f },
                        /* size             */ cardCornerSize,
                        /* rotation degrees */ 90.0f,
                        /* sweep direction  */ D2D1_SWEEP_DIRECTION_CLOCKWISE,
                        /* arc size         */ D2D1_ARC_SIZE_SMALL
                    });
                    geoSink->AddLine({ cardWidth -  cardRoundRadius * 2.0f, 0.0f });

                    // Right Top Corner
                    geoSink->AddArc(
                    {
                        /* point            */ { cardWidth - cardRoundRadius, cardRoundRadius },
                        /* size             */ cardCornerSize,
                        /* rotation degrees */ 90.0f,
                        /* sweep direction  */ D2D1_SWEEP_DIRECTION_CLOCKWISE,
                        /* arc size         */ D2D1_ARC_SIZE_SMALL
                    });
                    geoSink->AddLine({ cardWidth - cardRoundRadius, cardHeight - cardRoundRadius });

                    // Right Bottom Corner
                    geoSink->AddArc(
                    {
                        /* point            */ { cardWidth, cardHeight },
                        /* size             */ cardCornerSize,
                        /* rotation degrees */ 90.0f,
                        /* sweep direction  */ D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
                        /* arc size         */ D2D1_ARC_SIZE_SMALL
                    });
                    geoSink->AddLine({ 0.0f, cardHeight });

                    geoSink->EndFigure(D2D1_FIGURE_END_CLOSED);
                }
                THROW_IF_FAILED(geoSink->Close());

                resource_utils::g_solidColorBrush->SetColor(setting.background.color);
                resource_utils::g_solidColorBrush->SetOpacity(setting.background.opacity);

                rndr->d2d1DeviceContext()->FillGeometry(pathGeo.Get(), resource_utils::g_solidColorBrush.Get());
            }
            activeCardShadow.endShadowDraw(rndr->d2d1DeviceContext());
        }
    }

    void TabGroup::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        // Card-Bar Panel
        {
            auto& setting = getAppearance().tabBar.card.main[(size_t)CardState::Dormant];

            resource_utils::g_solidColorBrush->SetColor(setting.background.color);
            resource_utils::g_solidColorBrush->SetOpacity(setting.background.opacity);

            rndr->d2d1DeviceContext()->FillRoundedRectangle(
            {
                cardBarAbsoluteRect(),
                setting.geometry.roundRadius,
                setting.geometry.roundRadius
            },
            resource_utils::g_solidColorBrush.Get());
        }
        // Hover-Card
        if (m_currHoverCardTabIndex.valid())
        {
            if (m_currHoverCardTabIndex != m_currActiveCardTabIndex)
            {
                auto& setting = getAppearance().tabBar.card.main[(size_t)CardState::Hover];

                resource_utils::g_solidColorBrush->SetColor(setting.background.color);
                resource_utils::g_solidColorBrush->SetOpacity(setting.background.opacity);

                rndr->d2d1DeviceContext()->FillRoundedRectangle(
                {
                    cardAbsoluteRect(m_currHoverCardTabIndex),
                    setting.geometry.roundRadius,
                    setting.geometry.roundRadius
                },
                resource_utils::g_solidColorBrush.Get());
            }
        }
        // Active-Card
        if (m_currActiveCardTabIndex.valid())
        {
            // Shadow
            activeCardShadow.color = getAppearance().tabBar.card.activeShadowColor;

            activeCardShadow.configShadowEffectInput(resource_utils::g_shadowEffect.Get());

            auto shadowPosition = math_utils::roundf(math_utils::leftTop(cardAbsoluteRect(m_currActiveCardTabIndex)));

            rndr->d2d1DeviceContext()->DrawImage(resource_utils::g_shadowEffect.Get(), shadowPosition);
            
            // Entity
            auto& setting = getAppearance().tabBar.card.main[(size_t)CardState::Active];

            rndr->d2d1DeviceContext()->DrawBitmap(
                activeCardShadow.bitmap.Get(), cardAbsoluteRect(m_currActiveCardTabIndex), setting.background.opacity);
        }
        // Background
        {
            auto& background = getAppearance().background;

            resource_utils::g_solidColorBrush->SetColor(background.color);
            resource_utils::g_solidColorBrush->SetOpacity(background.opacity);

            ResizablePanel::drawBackground(rndr);
        }
        // Content
        if (m_currActiveCardTabIndex.valid())
        {
            if (m_currActiveCardTabIndex->content->isD2d1ObjectVisible())
            {
                m_currActiveCardTabIndex->content->onRendererDrawD2d1Object(rndr);
            }
        }
        // Miscellaneous
        {
            for (TabIndex tabIndex = { &m_tabs, 0 }; tabIndex < m_candidateTabCount; ++tabIndex)
            {
                auto currState = getCardState(tabIndex);
                auto nextState = getCardState(tabIndex.getNext());

                // Card Controls
                {
                    // Caption
                    if (tabIndex->caption->isD2d1ObjectVisible())
                    {
                        tabIndex->caption->onRendererDrawD2d1Object(rndr);
                    }
                }
                // Separators
                if (currState == CardState::Dormant && nextState == CardState::Dormant)
                {
                    if (tabIndex != m_candidateTabCount - 1)
                    {
                        auto& setting = getAppearance().tabBar.separator;

                        resource_utils::g_solidColorBrush->SetColor(setting.background.color);
                        resource_utils::g_solidColorBrush->SetOpacity(setting.background.opacity);

                        rndr->d2d1DeviceContext()->FillRectangle(
                            separatorAbsoluteRect(tabIndex), resource_utils::g_solidColorBrush.Get());
                    }
                }
            }
            // More-Cards Control
            {
                auto state = getMoreCardsButtonState();
                auto& setting = getAppearance().tabBar.moreCards.control;

                // Button
                auto& buttonBackground = setting.button.background[(size_t)state];

                resource_utils::g_solidColorBrush->SetColor(buttonBackground.color);
                resource_utils::g_solidColorBrush->SetOpacity(buttonBackground.opacity);

                rndr->d2d1DeviceContext()->FillRoundedRectangle(
                {
                    moreCardsButtonAbsoluteRect(),
                    setting.button.geometry.roundRadius,
                    setting.button.geometry.roundRadius
                },
                resource_utils::g_solidColorBrush.Get());

                // Icon
                auto& iconBackground = setting.icon.background[(size_t)state];

                resource_utils::g_solidColorBrush->SetColor(iconBackground.color);
                resource_utils::g_solidColorBrush->SetOpacity(iconBackground.opacity);

                rndr->d2d1DeviceContext()->FillRectangle(
                    moreCardsIconAbsoluteRect(), resource_utils::g_solidColorBrush.Get());

                ComPtr<ID2D1PathGeometry> pathGeo;
                THROW_IF_FAILED(rndr->d2d1Factory()->CreatePathGeometry(&pathGeo));

                ComPtr<ID2D1GeometrySink> geoSink;
                THROW_IF_FAILED(pathGeo->Open(&geoSink));
                {
                    auto triangleVertices = moreCardsIconAbsoluteTriangle();

                    geoSink->BeginFigure(triangleVertices[2], D2D1_FIGURE_BEGIN_FILLED);

                    geoSink->AddLines(triangleVertices.data(), (UINT32)triangleVertices.size());

                    geoSink->EndFigure(D2D1_FIGURE_END_CLOSED);
                }
                THROW_IF_FAILED(geoSink->Close());

                rndr->d2d1DeviceContext()->FillGeometry(pathGeo.Get(), resource_utils::g_solidColorBrush.Get());
            }
        }
        // Mask when Belowing Dragged Window
        if (isAssociatedWindowDraggedAbove())
        {
            auto& maskSetting = getAppearance().maskWhenBelowDragWindow;

            resource_utils::g_solidColorBrush->SetColor(maskSetting.color);
            resource_utils::g_solidColorBrush->SetOpacity(maskSetting.opacity);

            rndr->d2d1DeviceContext()->FillRoundedRectangle(
                { m_absoluteRect, roundRadiusX, roundRadiusY }, resource_utils::g_solidColorBrush.Get());
        }
    }

    void TabGroup::drawD2d1ObjectPosterior(renderer::Renderer* rndr)
    {
        // Outline
        resource_utils::g_solidColorBrush->SetColor(getAppearance().stroke.color);
        resource_utils::g_solidColorBrush->SetOpacity(getAppearance().stroke.opacity);

        // In general, the round-radius of the tab-group is 0, so we can draw
        // left, right and bottom lines separately to hide the topmost border.

        auto leftTop = absolutePosition();

        float selfWidth = width();
        float selfHeight = height();

        auto& stroke = getAppearance().stroke;

        auto point00 = math_utils::offset(leftTop, { stroke.width * 0.5f, 0.0f });
        auto point01 = math_utils::offset(point00, { 0.0f, selfHeight });

        auto point10 = math_utils::offset(leftTop, { selfWidth - stroke.width * 0.5f, 0.0f });
        auto point11 = math_utils::offset(point10, { 0.0f, selfHeight });

        auto point20 = math_utils::offset(leftTop, { 0.0f, selfHeight - stroke.width * 0.5f });
        auto point21 = math_utils::offset(point20, { selfWidth, 0.0f });

        rndr->d2d1DeviceContext()->DrawLine(
            point00, point01,
            resource_utils::g_solidColorBrush.Get(),
            stroke.width);

        rndr->d2d1DeviceContext()->DrawLine(
            point10, point11,
            resource_utils::g_solidColorBrush.Get(),
            stroke.width);

        rndr->d2d1DeviceContext()->DrawLine(
            point20, point21,
            resource_utils::g_solidColorBrush.Get(),
            stroke.width);

        // Content's Posterior Objects
        if (m_currActiveCardTabIndex.valid())
        {
            if (m_currActiveCardTabIndex->content->isD2d1ObjectVisible())
            {
                m_currActiveCardTabIndex->content->drawD2d1ObjectPosterior(rndr);
            }
        }
        ResizablePanel::drawD2d1ObjectPosterior(rndr);
    }

    float TabGroup::minimalWidth() const
    {
        float minWidth = getAppearance().tabBar.geometry.rightPadding;

        if (m_currActiveCardTabIndex.valid())
        {
            minWidth += absoluteToSelfCoord(cardAbsoluteRect(m_currActiveCardTabIndex)).right;
        }
        return minWidth;
    }

    bool TabGroup::isHitHelper(const Event::Point& p) const
    {
        return math_utils::isOverlapped(p, sizingFrameExtendedRect(cardBarExtendedAbsoluteRect()));
    }

    void TabGroup::onSizeHelper(SizeEvent& e)
    {
        ResizablePanel::onSizeHelper(e);

        if (m_currActiveCardTabIndex.valid())
        {
            m_currActiveCardTabIndex->content->resize(size());
        }
        updateCandidateTabInfo();
    }

    void TabGroup::onMoveHelper(MoveEvent& e)
    {
        ResizablePanel::onMoveHelper(e);

        updateCandidateTabInfo();
    }

    void TabGroup::onChangeThemeHelper(WstrParam themeName)
    {
        ResizablePanel::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(themeName);

        for (auto& tab : m_tabs)
        {
            if (tab.m_previewItem->parent().expired())
            {
                tab.m_previewItem->onChangeTheme(themeName);
            }
        } // The managed preview-items will be updated by the preview-panel.
    }

    void TabGroup::onMouseMoveHelper(MouseMoveEvent& e)
    {
        ResizablePanel::onMouseMoveHelper(e);

        auto& p = e.cursorPoint;

        if (math_utils::isOverlapped(p, cardBarExtendedCardBarAbsoluteRect()))
        {
            if (m_currDraggedCardTabIndex.valid())
            {
                if (e.buttonState.leftPressed)
                {
                    auto nextCardTabIndex = selfcoordOffsetToCardTabIndex(absoluteToSelfCoord(p).x);

                    if (nextCardTabIndex.valid() && nextCardTabIndex != m_currDraggedCardTabIndex)
                    {
                        swapTab(m_currDraggedCardTabIndex, nextCardTabIndex);

                        selectTab(nextCardTabIndex); // m_currActiveCardTabIndex is updated in this.

                        m_currHoverCardTabIndex = m_currDraggedCardTabIndex = nextCardTabIndex;
                    }
                }
            }
            else // No card being dragged.
            {
                m_currHoverCardTabIndex.invalidate();

                for (TabIndex tabIndex = { &m_tabs, 0 }; tabIndex < m_candidateTabCount; ++tabIndex)
                {
                    if (math_utils::isInside(p, cardAbsoluteRect(tabIndex)))
                    {
                        m_currHoverCardTabIndex = tabIndex;
                    }
                }
                if (!math_utils::isInside(p, moreCardsButtonAbsoluteRect()))
                {
                    m_isMoreCardsButtonHover = false;
                    m_isMoreCardsButtonDown = false;
                }
                else m_isMoreCardsButtonHover = true;
            }
        }
        else // Not above the card-bar.
        {
            m_currHoverCardTabIndex.invalidate();

            if (e.buttonState.leftPressed)
            {
                triggerTabPromoting(e);
            }
            m_currDraggedCardTabIndex.invalidate();

            m_isMoreCardsButtonHover = false;
            m_isMoreCardsButtonDown = false;
        }
    }

    void TabGroup::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        ResizablePanel::onMouseLeaveHelper(e);

        m_currHoverCardTabIndex.invalidate();

        if (e.buttonState.leftPressed)
        {
            triggerTabPromoting(e);
        }
        m_currDraggedCardTabIndex.invalidate();

        m_isMoreCardsButtonHover = false;
        m_isMoreCardsButtonDown = false;
    }

    void TabGroup::onMouseButtonHelper(MouseButtonEvent& e)
    {
        ResizablePanel::onMouseButtonHelper(e);

        auto& p = e.cursorPoint;

        if (e.state.leftDown() || e.state.leftDblclk())
        {
            if (m_currHoverCardTabIndex.valid())
            {
                if (m_currHoverCardTabIndex->caption->draggable &&
                   (!m_currHoverCardTabIndex->caption->closable ||
                    !m_currHoverCardTabIndex->caption->m_isCloseButtonHover))
                {
                    m_currDraggedCardTabIndex = m_currHoverCardTabIndex;
                }
            }
            m_isMoreCardsButtonDown = m_isMoreCardsButtonHover;
        }
        else if (e.state.leftUp())
        {
            if (m_isMoreCardsButtonDown)
            {
                m_isMoreCardsButtonDown = false;

                updatePreviewPanelItems();
                m_previewPanel->setActivated(true);
            }
            m_currDraggedCardTabIndex.invalidate();
        }
    }
}
