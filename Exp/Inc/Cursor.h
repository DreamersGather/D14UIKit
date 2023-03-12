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
        void setIcon(StaticIconIndex index);
        void setIcon(DynamicIconIndex index);

    protected:
        explicit Cursor(Passkey);

        void initialize();
    };
}
