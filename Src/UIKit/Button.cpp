#include "Common/Precompile.h"

#include "UIKit/Button.h"

#include "Common/CppLangUtils/PointerEquality.h"
#include "Common/MathUtils/2D.h"

#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    Button::Button(
        ShrdPtrParam<IconLabel> content,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        ClickablePanel(rect, resource_utils::g_solidColorBrush),
        m_content(content)
    {
        m_takeOverChildrenDrawing = true;

        roundRadiusX = roundRadiusY = roundRadius;
    }

    Button::Button(
        WstrParam text,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        Button(
            IconLabel::uniformLayout(text),
            roundRadius,
            rect) { }

    void Button::onInitializeFinish()
    {
        ClickablePanel::onInitializeFinish();

        if (!m_content)
        {
            m_content = IconLabel::uniformLayout();
        }
        addUIObject(m_content);

        m_content->transform(selfCoordRect());
    }

    void Button::setEnabled(bool value)
    {
        Panel::setEnabled(value);

        m_currState = value ? State::Idle : State::Disabled;

        m_content->setEnabled(value);
    }

    const SharedPtr<IconLabel>& Button::content() const
    {
        return m_content;
    }

    void Button::setContent(ShrdPtrParam<IconLabel> content)
    {
        if (content && !cpp_lang_utils::isMostDerivedEqual(content, m_content))
        {
            removeUIObject(m_content);

            m_content = content;
            addUIObject(m_content);

            m_content->transform(selfCoordRect());
        }
    }

    void Button::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        // Background
        auto& bkgn = getAppearance().background;

        resource_utils::g_solidColorBrush->SetColor(bkgn.color);
        resource_utils::g_solidColorBrush->SetOpacity(bkgn.opacity);

        ClickablePanel::drawBackground(rndr);

        // Content
        auto& foreground = getAppearance().foreground;

        m_content->icon.bitmapOpacity = foreground.opacity;
        m_content->label()->getAppearance().foreground = foreground;

        if (m_content->isD2d1ObjectVisible())
        {
            m_content->onRendererDrawD2d1Object(rndr);
        }
        // Outline
        auto& stroke = getAppearance().stroke;

        resource_utils::g_solidColorBrush->SetColor(stroke.color);
        resource_utils::g_solidColorBrush->SetOpacity(stroke.opacity);

        auto frame = math_utils::inner(m_absoluteRect, stroke.width);
        D2D1_ROUNDED_RECT outlineRect = { frame, roundRadiusX, roundRadiusY };

        rndr->d2d1DeviceContext()->DrawRoundedRectangle(
            outlineRect, resource_utils::g_solidColorBrush.Get(), stroke.width);
    }

    bool Button::isHitHelper(const Event::Point& p) const
    {
        return math_utils::isInside(p, m_absoluteRect);
    }

    bool Button::destroyUIObjectHelper(ShrdPtrParam<Panel> uiobj)
    {
        if (cpp_lang_utils::isMostDerivedEqual(m_content, uiobj)) return false;

        return Panel::destroyUIObjectHelper(uiobj);
    }

    void Button::onSizeHelper(SizeEvent& e)
    {
        ClickablePanel::onSizeHelper(e);

        m_content->transform(selfCoordRect());
    }

    void Button::onChangeThemeHelper(WstrParam themeName)
    {
        ClickablePanel::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(themeName);
    }

    void Button::onMouseEnterHelper(MouseMoveEvent& e)
    {
        ClickablePanel::onMouseEnterHelper(e);

        m_currState = State::Hover;
    }

    void Button::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        ClickablePanel::onMouseLeaveHelper(e);

        m_currState = State::Idle;
    }

    void Button::onMouseButtonPressHelper(Event& e)
    {
        ClickablePanel::onMouseButtonPressHelper(e);

        if (e.left()) m_currState = State::Down;
    }

    void Button::onMouseButtonReleaseHelper(Event& e)
    {
        ClickablePanel::onMouseButtonReleaseHelper(e);

        if (e.left()) m_currState = State::Hover;
    }
}
