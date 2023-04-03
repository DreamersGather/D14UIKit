#include "Common/Precompile.h"

#include "UIKit/IconLabel2.h"

#include "Common/CppLangUtils/PointerEquality.h"
#include "Common/DirectXError.h"
#include "Common/MathUtils/2D.h"

#include "UIKit/Label.h"

namespace d14engine::uikit
{
    IconLabel2::IconLabel2(
        WstrParam labelText, WstrParam label2Text,
        ComPtrParam<ID2D1Bitmap1> iconBitmap,
        float iconBitmapOpacity,
        const D2D1_RECT_F& rect)
        :
        IconLabel(labelText, iconBitmap, iconBitmapOpacity, rect)
    {
        m_label2 = makeUIObject<Label>(label2Text);
    }

    void IconLabel2::onInitializeFinish()
    {
        IconLabel::onInitializeFinish();

        addUIObject(m_label2);

        m_label2->transform(selfCoordRect());
    }

    void IconLabel2::setEnabled(bool value)
    {
        IconLabel::setEnabled(value);

        m_label2->setEnabled(value);
    }

    const SharedPtr<Label>& IconLabel2::label2() const
    {
        return m_label2;
    }

    void IconLabel2::setLabel2(ShrdPtrParam<Label> label)
    {
        if (label && !cpp_lang_utils::isMostDerivedEqual(label, m_label2))
        {
            removeUIObject(m_label2);

            m_label2 = label;
            addUIObject(m_label2);

            m_label2->transform(selfCoordRect());
        }
    }

    SharedPtr<IconLabel2> IconLabel2::menuItemLayout(
        WstrParam labelText,
        WstrParam hotkeyText,
        ComPtrParam<ID2D1Bitmap1> iconBitmap,
        float iconBitmapOpacity,
        float textHeadPadding,
        float hotkeyTailPadding,
        const D2D1_RECT_F& rect)
    {
        auto iconLabel = makeUIObject<IconLabel2>(labelText, hotkeyText, iconBitmap, iconBitmapOpacity, rect);

        THROW_IF_FAILED(iconLabel->m_label2->textLayout()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING));

        iconLabel->f_updateLayout = [textHeadPadding, hotkeyTailPadding](IconLabel* pIconLabel)
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
            pIconLabel->icon.rect = math_utils::centered(
            {
                0.0f,
                0.0f,
                textHeadPadding,
                pIconLabel->height()
            },
            iconSize);

            D2D1_RECT_F textRect =
            {
                textHeadPadding,
                0.0f,
                pIconLabel->width() - hotkeyTailPadding,
                pIconLabel->height()
            };
            pIconLabel->label()->transform(textRect);
            ((IconLabel2*)pIconLabel)->m_label2->transform(textRect);
        };
        // Perform the callback immediately to initialize the layout.
        iconLabel->f_updateLayout(iconLabel.get());

        return iconLabel;
    }

    void IconLabel2::onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr)
    {
        // Label & Icon
        IconLabel::onRendererDrawD2d1ObjectHelper(rndr);

        // Label-2 Text
        if (m_label2->isD2d1ObjectVisible())
        {
            m_label2->onRendererDrawD2d1Object(rndr);
        }
    }

    bool IconLabel2::destroyUIObjectHelper(ShrdPtrParam<Panel> uiobj)
    {
        if (cpp_lang_utils::isMostDerivedEqual(uiobj, m_label2)) return false;

        return IconLabel::destroyUIObjectHelper(uiobj);
    }
}
