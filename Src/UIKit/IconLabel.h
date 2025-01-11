#pragma once

#include "Common/Precompile.h"

#include "UIKit/BitmapObject.h"
#include "UIKit/Panel.h"

namespace d14engine::uikit
{
    struct Label;

    struct IconLabel : Panel
    {
        IconLabel(
            WstrParam labelText = {},
            BmpObjParam iconBitmap = {},
            const D2D1_RECT_F& rect = {});

        void onInitializeFinish() override;

        void setEnabled(bool value) override;

    public:
        struct Icon
        {
            D2D1_RECT_F rect = {};
            BitmapObject bitmap = {};

            Optional<D2D1_SIZE_F> customSize = std::nullopt;
        }
        icon = {};

    protected:
        SharedPtr<Label> m_label = {};

    public:
        const SharedPtr<Label>& label() const;
        void setLabel(ShrdPtrParam<Label> label);

    public:
        void updateLayout();

        Function<void(IconLabel*)> f_updateLayout = {};

    protected:
        virtual void updateLayoutHelper();

    public:
        static SharedPtr<IconLabel> uniformLayout(
            WstrParam labelText = {},
            BmpObjParam iconBitmap = {},
            const D2D1_RECT_F& rect = {});

        static SharedPtr<IconLabel> compactLayout(
            WstrParam labelText = {},
            BmpObjParam iconBitmap = {},
            float iconHeadPadding = 0.0f,
            float iconTailPadding = 0.0f,
            const D2D1_RECT_F& rect = {});

        static SharedPtr<IconLabel> iconExpandedLayout(
            WstrParam labelText = {},
            BmpObjParam iconBitmap = {},
            const D2D1_RECT_F& rect = {});

        static SharedPtr<IconLabel> labelExpandedLayout(
            WstrParam labelText = {},
            BmpObjParam iconBitmap = {},
            const D2D1_RECT_F& rect = {});

        static SharedPtr<IconLabel> comboBoxLayout(
            WstrParam labelText = {},
            BmpObjParam iconBitmap = {},
            const D2D1_RECT_F& rect = {});

    protected:
        void onSizeHelper(SizeEvent& e) override;
        bool releaseUIObjectHelper(ShrdPtrParam<Panel> uiobj) override;
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;
    };
}
