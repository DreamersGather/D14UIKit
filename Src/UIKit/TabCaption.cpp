#include "Common/Precompile.h"

#include "UIKit/TabCaption.h"

#include "Common/CppLangUtils/PointerEquality.h"

#include "UIKit/Application.h"
#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/ResourceUtils.h"
#include "UIKit/TabGroup.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    TabCaption::TabCaption(ShrdPtrParam<IconLabel> title)
        :
        m_title(title) { }

    TabCaption::TabCaption(WstrParam text)
        :
        TabCaption(IconLabel::compactLayout(text)) { }

    void TabCaption::onInitializeFinish()
    {
        Panel::onInitializeFinish();

        if (!m_title)
        {
            m_title = IconLabel::compactLayout(L"Untitled");
        }
        addUIObject(m_title);

        m_title->transform(titleSelfcoordRect());

        m_title->label()->drawTextOptions = D2D1_DRAW_TEXT_OPTIONS_CLIP;
    }

    void TabCaption::setEnabled(bool value)
    {
        Panel::setEnabled(value);

        m_title->setEnabled(value);
    }

    const SharedPtr<IconLabel>& TabCaption::title() const
    {
        return m_title;
    }

    void TabCaption::setTitle(ShrdPtrParam<IconLabel> title)
    {
        if (title && !cpp_lang_utils::isMostDerivedEqual(title, m_title))
        {
            removeUIObject(m_title);

            m_title = title;
            addUIObject(m_title);

            m_title->transform(titleSelfcoordRect());
        }
    }

    const WeakPtr<TabGroup>& TabCaption::parentTabGroup() const
    {
        return m_parentTabGroup;
    }

    D2D1_RECT_F TabCaption::titleSelfcoordRect() const
    {
        return math_utils::increaseLeftRight(selfCoordRect(),
            { getAppearance().title.leftPadding, -getAppearance().title.rightPadding });
    }

    D2D1_RECT_F TabCaption::closeIconAbsoluteRect(const D2D1_RECT_F& buttonRect) const
    {
        return math_utils::rect(math_utils::offset(math_utils::leftTop(buttonRect),
            getAppearance().closeX.icon.geometry.offset), getAppearance().closeX.icon.geometry.size);
    }

    D2D1_RECT_F TabCaption::closeButtonAbsoluteRect() const
    {
        return math_utils::rect(math_utils::offset(math_utils::rightTop(m_absoluteRect),
        {
            getAppearance().closeX.button.geometry.rightOffset,
            (height() - getAppearance().closeX.button.geometry.size.height) * 0.5f
        }),
        getAppearance().closeX.button.geometry.size); // keep the button always vertical centrally
    }

    TabCaption::ButtonState TabCaption::getCloseButtonState() const
    {
        if (m_isCloseButtonDown) return ButtonState::Down;
        else if (m_isCloseButtonHover) return ButtonState::Hover;
        else return ButtonState::Idle;
    }

    void TabCaption::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        if (closable)
        {
            auto stateIndex = (size_t)getCloseButtonState();

            auto& buttonBackground = getAppearance().closeX.button.background[stateIndex];
            auto& iconBackground = getAppearance().closeX.icon.background[stateIndex];

            // Close Button
            resource_utils::g_solidColorBrush->SetColor(buttonBackground.color);
            resource_utils::g_solidColorBrush->SetOpacity(buttonBackground.opacity);

            auto buttonRect = closeButtonAbsoluteRect();

            rndr->d2d1DeviceContext()->FillRoundedRectangle(
            {
                buttonRect,
                getAppearance().closeX.button.geometry.roundRadius,
                getAppearance().closeX.button.geometry.roundRadius
            },
            resource_utils::g_solidColorBrush.Get());

            // Close Icon
            resource_utils::g_solidColorBrush->SetColor(iconBackground.color);
            resource_utils::g_solidColorBrush->SetOpacity(iconBackground.opacity);

            auto iconRect = closeIconAbsoluteRect(buttonRect);

            rndr->d2d1DeviceContext()->DrawLine(
                math_utils::leftTop(iconRect), math_utils::rightBottom(iconRect),
                resource_utils::g_solidColorBrush.Get(), getAppearance().closeX.icon.strokeWidth);

            rndr->d2d1DeviceContext()->DrawLine(
                math_utils::rightTop(iconRect), math_utils::leftBottom(iconRect),
                resource_utils::g_solidColorBrush.Get(), getAppearance().closeX.icon.strokeWidth);
        }
    }

    bool TabCaption::isHitHelper(const Event::Point& p) const
    {
        return math_utils::isInside(p, m_absoluteRect);
    }

    void TabCaption::onSizeHelper(SizeEvent& e)
    {
        Panel::onSizeHelper(e);

        m_title->transform(titleSelfcoordRect());
    }

    void TabCaption::onChangeThemeHelper(WstrParam themeName)
    {
        Panel::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(themeName);
    }

    void TabCaption::onMouseMoveHelper(MouseMoveEvent& e)
    {
        Panel::onMouseMoveHelper(e);

        auto& p = e.cursorPoint;

        if (!math_utils::isOverlapped(p, closeButtonAbsoluteRect()))
        {
            m_isCloseButtonHover = false;
            m_isCloseButtonDown = false;
        }
        else m_isCloseButtonHover = true;
    }

    void TabCaption::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        Panel::onMouseLeaveHelper(e);

        m_isCloseButtonHover = false;
        m_isCloseButtonDown = false;
    }

    void TabCaption::onMouseButtonHelper(MouseButtonEvent& e)
    {
        Panel::onMouseButtonHelper(e);

        if (e.state.leftDown() || e.state.leftDblclk())
        {
            m_isCloseButtonDown = m_isCloseButtonHover;

            if ((!closable || !m_isCloseButtonHover) && !m_parentTabGroup.expired())
            {
                auto tabGroup = m_parentTabGroup.lock();

                size_t tabIndex = 0;
                for (auto& tab : tabGroup->tabs())
                {
                    if (cpp_lang_utils::isMostDerivedEqual(tab.caption, shared_from_this())) break;
                    ++tabIndex;
                }
                tabGroup->selectTab(tabIndex);

                Application::g_app->sendNextImmediateMouseMoveEvent = true;
            }
        }
        else if (e.state.leftUp())
        {
            if (m_isCloseButtonDown)
            {
                m_isCloseButtonDown = false;

                if (closable && !m_parentTabGroup.expired())
                {
                    auto tabGroup = m_parentTabGroup.lock();

                    size_t tabIndex = 0;
                    for (auto& tab : tabGroup->tabs())
                    {
                        if (cpp_lang_utils::isMostDerivedEqual(tab.caption, shared_from_this())) break;
                        ++tabIndex;
                    }
                    tabGroup->removeTab(tabIndex);

                    Application::g_app->sendNextImmediateMouseMoveEvent = true;
                }
            }
        }
    }
}
