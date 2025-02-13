#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnableMasterPtr.h"
#include "Common/CppLangUtils/IndexIterator.h"
#include "Common/MathUtils/2D.h"

#include "UIKit/Appearances/TabGroup.h"
#include "UIKit/ResizablePanel.h"
#include "UIKit/ShadowMask.h"

namespace d14engine::uikit
{
    struct MenuItem;
    struct PopupMenu;
    struct TabCaption;
    struct Window;

    struct TabGroup : appearance::TabGroup, ResizablePanel
    {
        explicit TabGroup(const D2D1_RECT_F& rect = {});

        virtual ~TabGroup();

        struct Tab { SharedPtr<TabCaption> caption; SharedPtr<Panel> content; };

        void onInitializeFinish() override;

        struct ActiveCard : cpp_lang_utils::EnableMasterPtr<TabGroup>
        {
            using EnableMasterPtr::EnableMasterPtr;

            ShadowMask mask = {};
            ComPtr<ID2D1PathGeometry> pathGeo = {};

            void loadMaskBitmap();
            void loadPathGeo();
        }
        activeCard{ this };

        D2D1_RECT_F cardBarExtendedAbsoluteRect() const;
        D2D1_RECT_F cardBarExtendedCardBarAbsoluteRect() const;

        _D14_SET_APPEARANCE_GETTER(TabGroup)

        float minimalWidth() const override;

    protected:
        struct TabImpl : Tab
        {
            friend struct TabGroup;

            TabImpl(const Tab& tab) : Tab(tab) { }

        private:
            SharedPtr<MenuItem> m_previewItem = {};

            D2D1_RECT_F m_cardAbsoluteRectCache = {};
        };
        using TabList = std::list<TabImpl>;

        TabList m_tabs = {};

        // Only the candidate tabs can be displayed in the card-bar, and the
        // remaining cards are listed in the more-cards preview-panel.
        size_t m_candidateTabCount = 0;

        using TabIndex = cpp_lang_utils::IndexIterator<TabList>;

        TabIndex selfcoordOffsetToCardTabIndex(float offset) const;

    public:
        using TabIndexParam = const TabIndex&;

        void onSelectedTabIndexChange(TabIndexParam index);

        Function<void(TabGroup*, TabIndexParam)> f_onSelectedTabIndexChange = {};

    protected:
        virtual void onSelectedTabIndexChangeHelper(TabIndexParam index);

    public:
        const TabList& tabs() const;

        void insertTab(const Tab& tab, size_t index = 0);
        void appendTab(const Tab& tab);

        void removeTab(size_t index, size_t count = 1);
        void clearAllTabs();

        void selectTab(size_t index);

        void swapTab(size_t index1, size_t index2);

    protected:
        void insertTab(const Tab& tab, TabIndexParam tabIndex);

        void removeTab(TabIndexParam tabIndex, size_t count);

        void selectTab(TabIndexParam tabIndex);

        void swapTab(TabIndexParam tabIndex1, TabIndexParam tabIndex2);

    protected:
        TabIndex m_currActiveCardTabIndex{};
        TabIndex m_currHoverCardTabIndex{};

        CardState getCardState(TabIndexParam tabIndex) const;

    public:
        const TabIndex& currActiveCardTabIndex() const;

    protected:
        bool m_isMoreCardsButtonHover = false;
        bool m_isMoreCardsButtonDown = false;

        ButtonState getMoreCardsButtonState() const;

    protected:
        SharedPtr<PopupMenu> m_previewPanel = {};

    public:
        const SharedPtr<PopupMenu>& previewPanel() const;

        // Call these when the card-geometry-environment changes.
        void updateCandidateTabInfo();
        void updatePreviewPanelItems();

    protected:
        D2D1_RECT_F cardBarAbsoluteRect() const;

        // Returns the cached value directly.
        const D2D1_RECT_F& cardAbsoluteRect(TabIndexParam tabIndex) const;

        D2D1_RECT_F cardCaptionAbsoluteRect(TabIndexParam tabIndex) const;

        D2D1_RECT_F separatorAbsoluteRect(TabIndexParam tabIndex) const;

        D2D1_RECT_F moreCardsIconAbsoluteRect() const;
        math_utils::Triangle2D moreCardsIconAbsoluteTriangle() const;
        D2D1_RECT_F moreCardsButtonAbsoluteRect() const;

    protected:
        TabIndex m_currDraggedCardTabIndex{};

    public:
        SharedPtr<Window> promoteTabToWindow(size_t index);

    protected:
        SharedPtr<Window> promoteTabToWindow(TabIndexParam tabIndex);

    public:
        Function<void(TabGroup*, Window*)> f_onTriggerTabPromoting = {};

        // When a window is being dragged, all of the tab-groups that have
        // been registered for the window will be associated with it, and if
        // the window is then released above any associated tab-group, its
        // caption and content will be demoted and inserted as a new tab and
        // the original window will be destroyed later.

        WeakPtr<Window> associatedWindow = {};

    protected:
        void triggerTabPromoting(MouseMoveEvent& e);

        bool isAssociatedWindowDraggedAbove() const;

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1LayerHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        void drawD2d1ObjectPosterior(renderer::Renderer* rndr) override;

        // Panel
        bool isHitHelper(const Event::Point& p) const override;

        void onSizeHelper(SizeEvent& e) override;

        void onMoveHelper(MoveEvent& e) override;

        void onChangeThemeHelper(WstrParam themeName) override;

        void onMouseMoveHelper(MouseMoveEvent& e) override;

        void onMouseLeaveHelper(MouseMoveEvent& e) override;

        void onMouseButtonHelper(MouseButtonEvent& e) override;
    };
}
