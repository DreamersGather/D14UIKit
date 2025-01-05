#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnumClassMap.h"

#include "UIKit/Appearances/ViewItem.h"
#include "UIKit/MaskStyle.h"
#include "UIKit/Panel.h"

namespace d14engine::uikit
{
    struct ViewItem : appearance::ViewItem, Panel
    {
        template<typename>
        friend struct WaterfallView;

        ViewItem(ShrdPtrParam<Panel> content, const D2D1_RECT_F& rect = {});

        ViewItem(WstrParam text = L"ViewItem", const D2D1_RECT_F& rect = {});

        void onInitializeFinish() override;

        MaskStyle contentMask = {};

        _D14_SET_APPEARANCE_GETTER(ViewItem)

        void setEnabled(bool value) override;

    protected:
        // NO WeakPtr here as we provide a content-param in the ctor.
        SharedPtr<Panel> m_content = {};

    public:
        template<typename T = Panel>
        WeakPtr<T> getContent() const
        {
            if constexpr (std::is_same_v<T, Panel>) return content();
            else return std::dynamic_pointer_cast<T>(content().lock());
        }
        WeakPtr<Panel> content() const;
        void setContent(ShrdPtrParam<Panel> content);

    public:
        State state = {};

        using StateTransitionMap = cpp_lang_utils::EnumClassMap<State>;

        const static StateTransitionMap
            ENTER_STATE_TRANS_MAP,
            LEAVE_STATE_TRANS_MAP,
            CHECK_STATE_TRANS_MAP,
            UNCHK_STATE_TRANS_MAP,
            GETFC_STATE_TRANS_MAP,
            LOSFC_STATE_TRANS_MAP;

        void triggerEnterStateTrans();
        void triggerLeaveStateTrans();
        void triggerCheckStateTrans();
        void triggerUnchkStateTrans();
        void triggerGetfcStateTrans();
        void triggerLosfcStateTrans();

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1LayerHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        bool isHitHelper(const Event::Point& p) const override;

        bool releaseUIObjectHelper(ShrdPtrParam<Panel> uiobj) override;

        void onSizeHelper(SizeEvent& e) override;

        void onChangeThemeHelper(WstrParam themeName) override;
    };
}
