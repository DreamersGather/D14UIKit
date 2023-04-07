#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnumClassMap.h"

#include "UIKit/AnimationUtils/Sequence.h"
#include "UIKit/Panel.h"

namespace d14engine::uikit
{
    struct Cursor : Panel
    {
        enum class StaticIconIndex
        {
            Alternate, Arrow, BackDiag, Beam, Hand, Help, HorzSize,
            MainDiag, Move, Pen, Person, Pin, Select, Stop, VertSize, Count
        };
        enum class DynamicIconIndex
        {
            Busy, Working, Count
        };
#define SET_STATIC(Name) constexpr static auto Name = StaticIconIndex::Name
#define SET_DYNAMIC(Name) constexpr static auto Name = DynamicIconIndex::Name

        SET_STATIC(Alternate);
        SET_STATIC(Arrow);
        SET_STATIC(BackDiag);
        SET_STATIC(Beam);
        SET_STATIC(Hand);
        SET_STATIC(Help);
        SET_STATIC(HorzSize);
        SET_STATIC(MainDiag);
        SET_STATIC(Move);
        SET_STATIC(Pen);
        SET_STATIC(Person);
        SET_STATIC(Pin);
        SET_STATIC(Select);
        SET_STATIC(Stop);
        SET_STATIC(VertSize);

        SET_DYNAMIC(Busy);
        SET_DYNAMIC(Working);

#undef SET_STATIC
#undef SET_DYNAMIC

        struct StaticIcon
        {
            D2D1_POINT_2F displayOffset = {};
            ComPtr<ID2D1Bitmap1> bitmap = {};
        };
        struct DynamicIcon
        {
            D2D1_POINT_2F displayOffset = {};
            animation_utils::DynamicBitmap bitmap = {};
        };

        using StaticIconMap = cpp_lang_utils::EnumClassMap<StaticIconIndex, StaticIcon>;
        using DynamicIconMap = cpp_lang_utils::EnumClassMap<DynamicIconIndex, DynamicIcon>;

        struct IconSeries
        {
            StaticIconMap staticIcons = {};
            DynamicIconMap dynamicIcons = {};
        };
        using BasicIconThemeMap = std::unordered_map<Wstring, IconSeries>;

        Cursor(
            const BasicIconThemeMap& icons = loadBasicIcons(),
            const D2D1_RECT_F& rect = { 0.0f, 0.0f, 32.0f, 32.0f });

        void registerDrawObjects() override;

    public:
        static BasicIconThemeMap loadBasicIcons();

    protected:
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
        // and its category will be decided by current themem automatically.

        void setIcon(StaticIconIndex index);
        void setStaticIcon(WstrParam name);

        void setIcon(DynamicIconIndex index);
        void setDynamicIcon(WstrParam name);

    protected:
        StaticIcon& getCurrentSelectedStaticIcon();
        DynamicIcon& getCurrentSelectedDynamicIcon();

    protected:
        // IDrawObject2D
        void onRendererUpdateObject2DHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;
    };
}
