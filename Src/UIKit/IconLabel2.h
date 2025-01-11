#pragma once

#include "Common/Precompile.h"

#include "IconLabel.h"

namespace d14engine::uikit
{
    struct IconLabel2 : IconLabel
    {
        IconLabel2(
            WstrParam labelText = {},
            WstrParam label2Text = {},
            BmpObjParam iconBitmap = {},
            const D2D1_RECT_F& rect = {});

        void onInitializeFinish() override;

        void setEnabled(bool value) override;

    protected:
        SharedPtr<Label> m_label2 = {};

    public:
        const SharedPtr<Label>& label2() const;
        void setLabel2(ShrdPtrParam<Label> label);

    public:
        static SharedPtr<IconLabel2> menuItemLayout(
            WstrParam labelText = {},
            WstrParam hotkeyText = {},
            BmpObjParam iconBitmap = {},
            float textHeadPadding = 30.0f,
            float hotkeyTailPadding = 30.0f,
            const D2D1_RECT_F& rect = {});

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;
        
        // Panel
        bool releaseUIObjectHelper(ShrdPtrParam<Panel> uiobj) override;
    };
}
