#include "Common/Precompile.h"

#include "UIKit/IconLabel.h"

#include "Common/CppLangUtils/PointerEquality.h"
#include "Common/DirectXError.h"
#include "Common/MathUtils/2D.h"

#include "Renderer/Renderer.h"

#include "UIKit/Label.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    IconLabel::IconLabel(
        WstrParam labelText,
        ComPtrParam<ID2D1Bitmap1> iconBitmap,
        float iconBitmapOpacity,
        const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush)
    {
        m_takeOverChildrenDrawing = true;

        icon.rect = selfCoordRect();
        icon.bitmap = iconBitmap;
        icon.bitmapOpacity = iconBitmapOpacity;

        m_label = makeUIObject<Label>(labelText);
    }

    void IconLabel::onInitializeFinish()
    {
        Panel::onInitializeFinish();

        addUIObject(m_label);

        m_label->transform(selfCoordRect());
    }

    void IconLabel::setEnabled(bool value)
    {
        Panel::setEnabled(value);

        m_label->setEnabled(value);
    }

    const SharedPtr<Label>& IconLabel::label() const
    {
        return m_label;
    }

    void IconLabel::setLabel(ShrdPtrParam<Label> label)
    {
        if (label && !cpp_lang_utils::isMostDerivedEqual(label, m_label))
        {
            removeUIObject(m_label);

            m_label = label;
            addUIObject(m_label);

            m_label->transform(selfCoordRect());
        }
    }

    void IconLabel::updateLayout()
    {
        if (f_updateLayout)
        {
            f_updateLayout(this);
        }
        else updateLayoutHelper();
    }

    void IconLabel::updateLayoutHelper()
    {
        // This method intentionally left blank.
    }

    SharedPtr<IconLabel> IconLabel::uniformLayout(
        WstrParam labelText,
        ComPtrParam<ID2D1Bitmap1> iconBitmap,
        float iconBitmapOpacity,
        const D2D1_RECT_F& rect)
    {
        auto iconLabel = makeUIObject<IconLabel>(labelText, iconBitmap, iconBitmapOpacity, rect);

        THROW_IF_FAILED(iconLabel->m_label->textLayout()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));

        iconLabel->f_updateLayout = [](IconLabel* pIconLabel)
        {
            if (pIconLabel->icon.bitmap)
            {
                D2D1_SIZE_F iconSize = {};
                if (pIconLabel->icon.customSize.has_value())
                {
                    iconSize = pIconLabel->icon.customSize.value();
                }
                else // use the original size of the bitmap
                {
                    iconSize = pIconLabel->icon.bitmap->GetSize();
                }
                float textWidth = pIconLabel->m_label->textMetrics().widthIncludingTrailingWhitespace;

                float spacing = pIconLabel->width() - iconSize.width - textWidth;
                float padding = std::max(spacing / 3.0f, 0.0f); // even interval

                float iconRectRight = std::min(padding * 2.0f + iconSize.width, pIconLabel->width());
                float labelRectLeft = std::min(padding + iconSize.width, pIconLabel->width());

                pIconLabel->icon.rect = math_utils::centered(
                {
                    0.0f,
                    0.0f,
                    iconRectRight,
                    pIconLabel->height()
                },
                iconSize);

                pIconLabel->m_label->transform(
                {
                    labelRectLeft,
                    0.0f,
                    pIconLabel->width(),
                    pIconLabel->height()
                });
            }
            else // Display the text in the center.
            {
                pIconLabel->icon.rect = math_utils::zeroRectF();
                pIconLabel->m_label->transform(pIconLabel->selfCoordRect());
            }
        };
        // Perform the callback immediately to initialize the layout.
        iconLabel->f_updateLayout(iconLabel.get());

        return iconLabel;
    }

    SharedPtr<IconLabel> IconLabel::compactLayout(
        WstrParam labelText,
        ComPtrParam<ID2D1Bitmap1> iconBitmap,
        float iconBitmapOpacity,
        float iconHeadPadding,
        float iconTailPadding,
        const D2D1_RECT_F& rect)
    {
        auto iconLabel = makeUIObject<IconLabel>(labelText, iconBitmap, iconBitmapOpacity, rect);

        iconLabel->f_updateLayout = [iconHeadPadding, iconTailPadding](IconLabel* pIconLabel)
        {
            D2D1_SIZE_F iconSize = { 0.0f, 0.0f };
            if (pIconLabel->icon.customSize.has_value())
            {
                iconSize = pIconLabel->icon.customSize.value();
            }
            else if (pIconLabel->icon.bitmap != nullptr)
            {
                iconSize = pIconLabel->icon.bitmap->GetSize();
            }
            float iconRectRight = std::min(iconHeadPadding + iconSize.width, pIconLabel->width());
            float labelRectLeft = std::min(iconRectRight + iconTailPadding, pIconLabel->width());

            pIconLabel->icon.rect = math_utils::centered(
            {
                iconHeadPadding,
                0.0f,
                iconRectRight,
                pIconLabel->height()
            },
            iconSize);

            pIconLabel->m_label->transform(
            {
                labelRectLeft,
                0.0f,
                pIconLabel->width(),
                pIconLabel->height()
            });
        };
        // Perform the callback immediately to initialize the layout.
        iconLabel->f_updateLayout(iconLabel.get());

        return iconLabel;
    }

    SharedPtr<IconLabel> IconLabel::iconExpandedLayout(
        WstrParam labelText,
        ComPtrParam<ID2D1Bitmap1> iconBitmap,
        float iconBitmapOpacity,
        const D2D1_RECT_F& rect)
    {
        auto iconLabel = makeUIObject<IconLabel>(labelText, iconBitmap, iconBitmapOpacity, rect);

        iconLabel->f_updateLayout = [](IconLabel* pIconLabel)
        {
            D2D1_SIZE_F iconSize = { 0.0f, 0.0f };
            if (pIconLabel->icon.customSize.has_value())
            {
                iconSize = pIconLabel->icon.customSize.value();
            }
            else if (pIconLabel->icon.bitmap != nullptr)
            {
                iconSize = pIconLabel->icon.bitmap->GetSize();
            }
            float textWidth = pIconLabel->m_label->textMetrics().width;
            float boundaryOffset = pIconLabel->width() - std::min(textWidth, pIconLabel->width());

            pIconLabel->icon.rect = math_utils::centered(
            {
                0.0f,
                0.0f,
                boundaryOffset,
                pIconLabel->height()
            },
            iconSize);

            pIconLabel->m_label->transform(
            {
                boundaryOffset,
                0.0f,
                pIconLabel->width(),
                pIconLabel->height()
            });
        };
        // Perform the callback immediately to initialize the layout.
        iconLabel->f_updateLayout(iconLabel.get());

        return iconLabel;
    }

    SharedPtr<IconLabel> IconLabel::labelExpandedLayout(
        WstrParam labelText,
        ComPtrParam<ID2D1Bitmap1> iconBitmap,
        float iconBitmapOpacity,
        const D2D1_RECT_F& rect)
    {
        auto iconLabel = makeUIObject<IconLabel>(labelText, iconBitmap, iconBitmapOpacity, rect);

        THROW_IF_FAILED(iconLabel->m_label->textLayout()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));

        iconLabel->f_updateLayout = [](IconLabel* pIconLabel)
        {
            D2D1_SIZE_F iconSize = { 0.0f, 0.0f };
            if (pIconLabel->icon.customSize.has_value())
            {
                iconSize = pIconLabel->icon.customSize.value();
            }
            else if (pIconLabel->icon.bitmap != nullptr)
            {
                iconSize = pIconLabel->icon.bitmap->GetSize();
            }
            float boundaryOffset = std::min(iconSize.width, pIconLabel->width());

            pIconLabel->icon.rect = math_utils::centered(
            {
                0.0f,
                0.0f,
                boundaryOffset,
                pIconLabel->height()
            },
            iconSize);

            pIconLabel->m_label->transform(
            {
                boundaryOffset,
                0.0f,
                pIconLabel->width(),
                pIconLabel->height()
            });
        };
        // Perform the callback immediately to initialize the layout.
        iconLabel->f_updateLayout(iconLabel.get());

        return iconLabel;
    }

    SharedPtr<IconLabel> IconLabel::comboBoxLayout(
        WstrParam labelText,
        ComPtrParam<ID2D1Bitmap1> iconBitmap,
        float iconBitmapOpacity,
        const D2D1_RECT_F& rect)
    {
        return compactLayout(labelText, iconBitmap, iconBitmapOpacity, 12.0f, 0.0f, rect);
    }

    void IconLabel::onSizeHelper(SizeEvent& e)
    {
        Panel::onSizeHelper(e);

        updateLayout();
    }

    bool IconLabel::destroyUIObjectHelper(ShrdPtrParam<Panel> uiobj)
    {
        if (cpp_lang_utils::isMostDerivedEqual(uiobj, m_label)) return false;

        return Panel::destroyUIObjectHelper(uiobj);
    }

    void IconLabel::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        // Label Text
        if (m_label->isD2d1ObjectVisible())
        {
            m_label->onRendererDrawD2d1Object(rndr);
        }
        // Icon Bitmap
        if (icon.bitmap)
        {
            auto rect = math_utils::roundf(selfCoordToAbsolute(icon.rect));

            rndr->d2d1DeviceContext()->DrawBitmap(
                icon.bitmap.Get(), rect, icon.bitmapOpacity);
        }
    }
}
