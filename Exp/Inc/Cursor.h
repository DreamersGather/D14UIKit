#pragma once

#include "Panel.h"

_D14_UIKIT_FWDEF(Cursor)

namespace d14uikit
{
    class DllExport Cursor : public Panel
    {
        friend class Application;

        _D14_UIKIT_PIMPL(Cursor)

        Cursor();

        enum class StaticIconIndex
        {
            Alternate,
            Arrow,
            BackDiag,
            Hand,
            Help,
            HorzSize,
            MainDiag,
            Move,
            Person,
            Pin,
            Select,
            Stop,
            Text,
            VertSize
        };
        enum class DynamicIconIndex
        {
            Busy,
            Working
        };
#define SET_STATIC(Name) constexpr static auto Name = StaticIconIndex::Name;
#define SET_DYNAMIC(Name) constexpr static auto Name = DynamicIconIndex::Name;

        SET_STATIC(Alternate)
        SET_STATIC(Arrow)
        SET_STATIC(BackDiag)
        SET_STATIC(Hand)
        SET_STATIC(Help)
        SET_STATIC(HorzSize)
        SET_STATIC(MainDiag)
        SET_STATIC(Move)
        SET_STATIC(Person)
        SET_STATIC(Pin)
        SET_STATIC(Select)
        SET_STATIC(Stop)
        SET_STATIC(Text)
        SET_STATIC(VertSize)

        SET_DYNAMIC(Busy)
        SET_DYNAMIC(Working)

#undef SET_STATIC
#undef SET_DYNAMIC

        enum class IconSource
        {
            System, UIKit
        };
        IconSource iconSource() const;
        void setIconSource(IconSource src);

#define SET_SOURCE(Name) constexpr static auto Name = IconSource::Name;

        SET_SOURCE(System)
        SET_SOURCE(UIKit)

#undef SET_SOURCE

        void setIcon(StaticIconIndex index);
        void setIcon(DynamicIconIndex index);
    };
}
