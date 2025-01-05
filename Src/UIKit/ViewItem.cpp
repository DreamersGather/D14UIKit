#include "Common/Precompile.h"

#include "UIKit/ViewItem.h"

#include "Common/CppLangUtils/PointerEquality.h"
#include "Common/MathUtils/2D.h"

#include "Renderer/Renderer.h"

#include "UIKit/IconLabel.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    ViewItem::ViewItem(
        ShrdPtrParam<Panel> content,
        const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        m_content(content)
    {
        m_takeOverChildrenDrawing = true;
    }

    ViewItem::ViewItem(WstrParam text, const D2D1_RECT_F& rect)
        : ViewItem(IconLabel::uniformLayout(text), rect) { }


    void ViewItem::onInitializeFinish()
    {
        Panel::onInitializeFinish();

        addUIObject(m_content);
        
        if (m_content) m_content->transform(selfCoordRect());
    }

    void ViewItem::setEnabled(bool value)
    {
        Panel::setEnabled(value);

        if (m_content) m_content->setEnabled(value);
    }

    WeakPtr<Panel> ViewItem::content() const
    {
        return m_content;
    }

    void ViewItem::setContent(ShrdPtrParam<Panel> content)
    {
        if (!cpp_lang_utils::isMostDerivedEqual(content, m_content))
        {
            removeUIObject(m_content);

            m_content = content;
            addUIObject(m_content);

            if (m_content) m_content->transform(selfCoordRect());
        }
    }

#define VIT_STATE ViewItem::State

    const ViewItem::StateTransitionMap
    ViewItem::ENTER_STATE_TRANS_MAP =
    {
        /* Idle                */ VIT_STATE::Hover,
        /* Hover               */ VIT_STATE::Hover,
        /* ActiveSelected      */ VIT_STATE::ActiveSelectedHover,
        /* InactiveSelected    */ VIT_STATE::InactiveSelected,
        /* ActiveSelectedHover */ VIT_STATE::ActiveSelectedHover
    },
    ViewItem::LEAVE_STATE_TRANS_MAP =
    {
        /* Idle                */ VIT_STATE::Idle,
        /* Hover               */ VIT_STATE::Idle,
        /* ActiveSelected      */ VIT_STATE::ActiveSelected,
        /* InactiveSelected    */ VIT_STATE::InactiveSelected,
        /* ActiveSelectedHover */ VIT_STATE::ActiveSelected
    },
    ViewItem::CHECK_STATE_TRANS_MAP =
    {
        /* Idle                */ VIT_STATE::ActiveSelectedHover,
        /* Hover               */ VIT_STATE::ActiveSelectedHover,
        /* ActiveSelected      */ VIT_STATE::ActiveSelectedHover,
        /* InactiveSelected    */ VIT_STATE::ActiveSelectedHover,
        /* ActiveSelectedHover */ VIT_STATE::ActiveSelectedHover
    },
    ViewItem::UNCHK_STATE_TRANS_MAP =
    {
        /* Idle                */ VIT_STATE::Idle,
        /* Hover               */ VIT_STATE::Idle,
        /* ActiveSelected      */ VIT_STATE::Idle,
        /* InactiveSelected    */ VIT_STATE::Idle,
        /* ActiveSelectedHover */ VIT_STATE::Idle
    },
    ViewItem::GETFC_STATE_TRANS_MAP =
    {
        /* Idle                */ VIT_STATE::Idle,
        /* Hover               */ VIT_STATE::Hover,
        /* ActiveSelected      */ VIT_STATE::ActiveSelected,
        /* InactiveSelected    */ VIT_STATE::ActiveSelected,
        /* ActiveSelectedHover */ VIT_STATE::ActiveSelectedHover
    },
    ViewItem::LOSFC_STATE_TRANS_MAP =
    {
        /* Idle                */ VIT_STATE::Idle,
        /* Hover               */ VIT_STATE::Hover,
        /* ActiveSelected      */ VIT_STATE::InactiveSelected,
        /* InactiveSelected    */ VIT_STATE::InactiveSelected,
        /* ActiveSelectedHover */ VIT_STATE::InactiveSelected
    };

#undef VIT_STATE

    void ViewItem::triggerEnterStateTrans()
    {
        state = ENTER_STATE_TRANS_MAP[(size_t)state];
    }

    void ViewItem::triggerLeaveStateTrans()
    {
        state = LEAVE_STATE_TRANS_MAP[(size_t)state];
    }

    void ViewItem::triggerCheckStateTrans()
    {
        state = CHECK_STATE_TRANS_MAP[(size_t)state];
    }

    void ViewItem::triggerUnchkStateTrans()
    {
        state = UNCHK_STATE_TRANS_MAP[(size_t)state];
    }

    void ViewItem::triggerGetfcStateTrans()
    {
        state = GETFC_STATE_TRANS_MAP[(size_t)state];
    }

    void ViewItem::triggerLosfcStateTrans()
    {
        state = LOSFC_STATE_TRANS_MAP[(size_t)state];
    }

    void ViewItem::onRendererDrawD2d1LayerHelper(Renderer* rndr)
    {
        if (m_content && m_content->isD2d1ObjectVisible())
        {
            m_content->onRendererDrawD2d1Layer(rndr);

            auto& setting = getAppearance().main[(size_t)(m_enabled ? state : State::Idle)];

            // Set opaque background to support rendering ClearType text.
            contentMask.color = setting.background.color;
            contentMask.color.a = setting.background.opacity;

            auto maskDrawTrans = D2D1::Matrix3x2F::Translation
            (
                -m_absoluteRect.left, -m_absoluteRect.top
            );
            contentMask.beginDraw(rndr->d2d1DeviceContext(), maskDrawTrans);
            {
                m_content->onRendererDrawD2d1Object(rndr);
            }
            contentMask.endDraw(rndr->d2d1DeviceContext());
        }
    }

    void ViewItem::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        auto& setting = getAppearance().main[(size_t)(m_enabled ? state : State::Idle)];

        // Background
        resource_utils::g_solidColorBrush->SetColor(setting.background.color);
        resource_utils::g_solidColorBrush->SetOpacity(setting.background.opacity);

        Panel::drawBackground(rndr);

        // Content
        if (m_content && m_content->isD2d1ObjectVisible())
        {
            rndr->d2d1DeviceContext()->DrawBitmap(
                contentMask.bitmap.Get(), math_utils::roundf(m_absoluteRect),
                contentMask.opacity, contentMask.getInterpolationMode());
        }
        // Outline
        resource_utils::g_solidColorBrush->SetColor(setting.stroke.color);
        resource_utils::g_solidColorBrush->SetOpacity(setting.stroke.opacity);

        auto frame = math_utils::inner(m_absoluteRect, setting.stroke.width);
        D2D1_ROUNDED_RECT outlineRect = { frame, roundRadiusX, roundRadiusY };

        rndr->d2d1DeviceContext()->DrawRoundedRectangle(
            outlineRect, resource_utils::g_solidColorBrush.Get(), setting.stroke.width);
    }

    bool ViewItem::isHitHelper(const Event::Point& p) const
    {
        // Generally, two adjacent view-items adjoin each other in vertical
        // direction, and WaterfallView judges that the curosr-point hits the
        // second one if it is right on the border of the view-items, so we
        // need to exclude the bottom-edge of the view-item from the hit-test.

        return math_utils::isOverlappedExcludingBottom(p, m_absoluteRect);
    }

    bool ViewItem::releaseUIObjectHelper(ShrdPtrParam<Panel> uiobj)
    {
        if (cpp_lang_utils::isMostDerivedEqual(m_content, uiobj)) m_content.reset();

        return Panel::releaseUIObjectHelper(uiobj);
    }

    void ViewItem::onSizeHelper(SizeEvent& e)
    {
        Panel::onSizeHelper(e);

        contentMask.loadBitmap(math_utils::roundu(e.size));

        if (m_content) m_content->transform(selfCoordRect());
    }

    void ViewItem::onChangeThemeHelper(WstrParam themeName)
    {
        Panel::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(themeName);
    }
}
