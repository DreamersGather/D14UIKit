#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnumMagic.h"

#include "UIKit/AnimationUtils/BitmapSequence.h"
#include "UIKit/Panel.h"

namespace d14engine::uikit
{
    struct Cursor : Panel
    {
        friend struct Application;

        enum class StaticIconIndex
        {
            Alternate, Arrow, BackDiag, Hand, Help, HorzSize,
            MainDiag, Move, Person, Pin, Select, Stop, Text, VertSize
        };
        enum class DynamicIconIndex
        {
            Busy, Working
        };
#define SET_STATIC_ALIAS(Name) constexpr static auto Name = StaticIconIndex::Name;
#define SET_DYNAMIC_ALIAS(Name) constexpr static auto Name = DynamicIconIndex::Name;

        SET_STATIC_ALIAS(Alternate)
        SET_STATIC_ALIAS(Arrow)
        SET_STATIC_ALIAS(BackDiag)
        SET_STATIC_ALIAS(Hand)
        SET_STATIC_ALIAS(Help)
        SET_STATIC_ALIAS(HorzSize)
        SET_STATIC_ALIAS(MainDiag)
        SET_STATIC_ALIAS(Move)
        SET_STATIC_ALIAS(Person)
        SET_STATIC_ALIAS(Pin)
        SET_STATIC_ALIAS(Select)
        SET_STATIC_ALIAS(Stop)
        SET_STATIC_ALIAS(Text)
        SET_STATIC_ALIAS(VertSize)

        SET_DYNAMIC_ALIAS(Busy)
        SET_DYNAMIC_ALIAS(Working)

#undef SET_STATIC_ALIAS
#undef SET_DYNAMIC_ALIAS

        template<typename BitmapData>
        struct Icon
        {
            BitmapData bitmapData = {};
            D2D1_POINT_2F hotSpotOffset = {};
        };
        using StaticIcon = Icon<BitmapObject>;
        using DynamicIcon = Icon<animation_utils::BitmapSequence>;

        using StaticIconMap = cpp_lang_utils::EnumMap<StaticIconIndex, StaticIcon>;
        using DynamicIconMap = cpp_lang_utils::EnumMap<DynamicIconIndex, DynamicIcon>;

        struct IconSeries
        {
            StaticIconMap staticIcons = {};
            DynamicIconMap dynamicIcons = {};
        };
        using BasicIconThemeMap = std::unordered_map<Wstring, IconSeries>;

        Cursor(
            const BasicIconThemeMap& icons = loadBasicIcons(),
            const D2D1_RECT_F& rect = { 0.0f, 0.0f, 48.0f, 48.0f });

        void registerDrawObjects() override;

    protected:
        static BasicIconThemeMap loadBasicIcons();

        static IconSeries loadBasicIconSeries(WstrParam themeName);
        static DynamicIcon loadBasicIconFrames(WstrParam name, size_t count);

    protected:
        BasicIconThemeMap m_classifiedBasicIcons = {};

        template<typename T>
        using IconLibrary = std::unordered_map<Wstring, T>;

        using StaticIconLibrary = IconLibrary<StaticIcon>;
        using DynamicIconLibrary = IconLibrary<DynamicIcon>;

        struct CustomIconSeries
        {
            StaticIconLibrary staticIcons = {};
            DynamicIconLibrary dynamicIcons = {};
        }
        m_customIcons = {};

    public:
        void registerIcon(WstrParam themeName, StaticIconIndex index, const StaticIcon& icon);

        void registerIcon(WstrParam name, const StaticIcon& icon);
        void unregisterStaticIcon(WstrParam name);

        void registerIcon(WstrParam themeName, DynamicIconIndex index, const DynamicIcon& icon);

        void registerIcon(WstrParam name, const DynamicIcon& icon);
        void unregisterDynamicIcon(WstrParam name);

    protected:
        template<typename T>
        using IconID = Variant<T, Wstring>;
        constexpr static size_t g_basicIconSeat = 0, g_customIconSeat = 1;

        using StaticIconID = IconID<StaticIconIndex>;
        using DynamicIconID = IconID<DynamicIconIndex>;

        using SelectedIconID = Variant<StaticIconID, DynamicIconID>;
        constexpr static size_t g_staticIconSeat = 0, g_dynamicIconSeat = 1;

        SelectedIconID m_selectedIconID = StaticIconIndex::Arrow;
        SelectedIconID m_lastSelectedIconID = StaticIconIndex::Arrow;

    public:
        // To show a basic icon, you only need to specify the index,
        // and its category will be decided by current theme automatically.
        //
        // For a custom icon, its ico-name is the unique identifier,
        // and you may need to manually adapt it in the onChangeThemeStyle.

        void setIcon(StaticIconIndex index);
        void setStaticIcon(WstrParam name);

        void setIcon(DynamicIconIndex index);
        void setDynamicIcon(WstrParam name);

    public:
        enum class IconSource { System, UIKit };

        constexpr static auto System = IconSource::System;
        constexpr static auto UIKit = IconSource::UIKit;

    protected:
        IconSource m_iconSource = System;

    public:
        IconSource iconSource() const;
        void setIconSource(IconSource src);

    protected:
        bool m_systemIconUpdateFlag = false;

        // Displays the corresponding system default cursor
        // based on the currently selected basic icon index.
        void setSystemIcon();

    protected:
        StaticIcon& getCurrentSelectedStaticIcon();
        DynamicIcon& getCurrentSelectedDynamicIcon();

    protected:
        // IDrawObject2D
        void onRendererUpdateObject2DHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;
    };
}
