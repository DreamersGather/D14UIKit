#include "Common/Precompile.h"

#include "UIKit/ComboBox.h"

#include "Common/CppLangUtils/PointerEquality.h"
#include "Common/DirectXError.h"
#include "Common/MathUtils/2D.h"

#include "UIKit/Application.h"
#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/PopupMenu.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    ComboBox::ComboBox(float roundRadius, const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        FlatButton(IconLabel::comboBoxLayout(), roundRadius, rect)
    {
        m_dropDownMenu = makeRootUIObject<PopupMenu>();

        m_dropDownMenu->f_onTriggerMenuItem = [this]
        (PopupMenu* menu, PopupMenu::ItemIndexParam itemIndex)
        {
            setCurrSelected(itemIndex.index);
        };
        m_dropDownMenu->setBackgroundTriggerPanel(true);

        auto& geoSetting = m_dropDownMenu->getAppearance().geometry;
        geoSetting.extension = geoSetting.roundRadius = roundRadius;

        // The shadow bitmap of the drop-down menu will be loaded later
        // in onInitializeFinish when resizing the menu to fit the size.
    }

    void ComboBox::onInitializeFinish()
    {
        FlatButton::onInitializeFinish();

        loadArrowIconStrokeStyle();

        m_dropDownMenu->resize(width(), m_dropDownMenu->height());
        m_dropDownMenu->move(m_absoluteRect.left, m_absoluteRect.bottom);
    }

    void ComboBox::loadArrowIconStrokeStyle()
    {
        auto factory = Application::g_app->dxRenderer()->d2d1Factory();

        auto properties = D2D1::StrokeStyleProperties
        (
            D2D1_CAP_STYLE_ROUND,
            D2D1_CAP_STYLE_ROUND,
            D2D1_CAP_STYLE_ROUND,
            D2D1_LINE_JOIN_MITER,
            10.0f, // miterLimit
            D2D1_DASH_STYLE_SOLID,
            0.0f   // dashOffset
        );
        THROW_IF_FAILED(factory->CreateStrokeStyle(
            properties, nullptr, 0, &arrowIcon.strokeStyle));
    }

    void ComboBox::onSelectedChange(IconLabel* content)
    {
        onSelectedChangeHelper(content);

        if (f_onSelectedChange) f_onSelectedChange(this, content);
    }

    void ComboBox::onSelectedChangeHelper(IconLabel* content)
    {
        // This method intentionally left blank.
    }

    const WeakPtr<MenuItem>& ComboBox::currSelected() const
    {
        return m_currSelected;
    }

    void ComboBox::setCurrSelected(size_t indexInDropDownMenu)
    {
        WeakPtr<MenuItem> originalSelected = m_currSelected;

        auto& items = m_dropDownMenu->childrenItems();
        if (indexInDropDownMenu >= 0 && indexInDropDownMenu < items.size())
        {
            auto itor = std::next(items.begin(), indexInDropDownMenu);
            auto newContent = (*itor)->getContent<IconLabel>().lock();

            m_currSelected = (*itor);
            if (newContent != nullptr)
            {
                m_content->icon = newContent->icon;
                m_content->label()->copyTextStyle(newContent->label().get(), newContent->label()->text());

                m_content->updateLayout();
                m_content->setVisible(true);
            }
        }
        else // Typically, pass SIZE_MAX to clear current selected item.
        {
            m_currSelected.reset();
            m_content->setVisible(false);
        }
        if (!cpp_lang_utils::isMostDerivedEqual(originalSelected.lock(), m_currSelected.lock()))
        {
            onSelectedChange(m_content.get());
        }
    }

    const SharedPtr<PopupMenu>& ComboBox::dropDownMenu() const
    {
        return m_dropDownMenu;
    }

    void ComboBox::setDropDownMenu(ShrdPtrParam<PopupMenu> menu)
    {
        if (menu && !cpp_lang_utils::isMostDerivedEqual(menu, m_dropDownMenu))
        {
            setCurrSelected(SIZE_MAX);

            m_dropDownMenu->destroy();
            m_dropDownMenu = menu;
        }
    }

    void ComboBox::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        FlatButton::onRendererDrawD2d1ObjectHelper(rndr);

        // Drop-down Arrow
        auto& arrowSetting = getAppearance().arrow;
        auto& arrowGeometry = arrowSetting.geometry;
        auto& arrowBackground = m_enabled ? arrowSetting.background : arrowSetting.secondaryBackground;

        resource_utils::g_solidColorBrush->SetColor(arrowBackground.color);
        resource_utils::g_solidColorBrush->SetOpacity(arrowBackground.opacity);

        auto arrowOrigin = math_utils::rightTop(m_absoluteRect);

        rndr->d2d1DeviceContext()->DrawLine(
            math_utils::offset(arrowOrigin, arrowGeometry.line0.point0),
            math_utils::offset(arrowOrigin, arrowGeometry.line0.point1),
            resource_utils::g_solidColorBrush.Get(),
            arrowSetting.strokeWidth, arrowIcon.strokeStyle.Get());

        rndr->d2d1DeviceContext()->DrawLine(
            math_utils::offset(arrowOrigin, arrowGeometry.line1.point0),
            math_utils::offset(arrowOrigin, arrowGeometry.line1.point1),
            resource_utils::g_solidColorBrush.Get(),
            arrowSetting.strokeWidth, arrowIcon.strokeStyle.Get());
    }

    void ComboBox::onSizeHelper(SizeEvent& e)
    {
        FlatButton::onSizeHelper(e);

        m_dropDownMenu->resize(e.size.width, m_dropDownMenu->height());
    }

    void ComboBox::onChangeThemeHelper(WstrParam themeName)
    {
        Button::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(FlatButton::getAppearance(), themeName);
    }

    void ComboBox::onMouseButtonReleaseHelper(ClickablePanel::Event& e)
    {
        FlatButton::onMouseButtonReleaseHelper(e);

        if (e.left())
        {
            if (!customMenuRelativePosition.has_value())
            {
                m_dropDownMenu->move(math_utils::offset(absolutePosition(),
                {
                    0.0f ,  m_currSelected.expired() ?
                    0.0f : -m_currSelected.lock()->position().y
                }));
            }
            else m_dropDownMenu->move(selfCoordToAbsolute(customMenuRelativePosition.value()));
            
            m_dropDownMenu->setActivated(true);
        }
    }
}
