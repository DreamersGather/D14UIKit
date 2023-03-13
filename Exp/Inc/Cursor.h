#pragma once

#include "Common.h"

#include "Panel.h"

namespace d14uikit
{
    class DllExport Cursor : public Panel
    {
        friend class Application;

        _D14_UIKIT_PIMPL_DEF

        Cursor();

        enum class StaticIconIndex
        {
            Alternate,
            Arrow,
            BackDiag,
            Beam,
            Hand,
            Help,
            HorzSize,
            MainDiag,
            Move,
            Pen,
            Person,
            Pin,
            Select,
            Stop,
            VertSize
        };
        enum class DynamicIconIndex
        {
            Busy,
            Working
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
        
        void setIcon(StaticIconIndex index);
        void setIcon(DynamicIconIndex index);

    protected:
        explicit Cursor(Passkey);

        void initialize();
    };
}
