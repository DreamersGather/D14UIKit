#pragma once

#include "Common/Precompile.h"

#include "Inc/BasicType.h"

#define _D14_UIKIT_CTOR(Type_Name) \
Type_Name::Type_Name(const std::shared_ptr<uikit::Type_Name>& uiobj)

// Binds d14engine objects to d14uikit objects:
// 
// It is necessary to use dynamic_pointer_cast, since some objects use virtual inheritance.
// Additionally, it is safe to do so without checking, as the source d14engine object must exist.
//
#define _D14_UIKIT_BIND(Type_Name, Obj_Name) do { \
    auto Obj_Name = pimpl->uiobj->Obj_Name()->shared_from_this(); \
    auto Obj_Name##Cast = std::dynamic_pointer_cast<uikit::Type_Name>(Obj_Name); \
    pimpl->Obj_Name = std::shared_ptr<Type_Name>(new Type_Name(Obj_Name##Cast)); \
} while (0)

namespace d14uikit
{
    Size convert(const D2D1_SIZE_F& size);
    D2D1_SIZE_F convert(const Size& size);

    Point convert(const D2D1_POINT_2F& p);
    D2D1_POINT_2F convert(const Point& p);

    Rect convert(const D2D1_RECT_F& rect);
    D2D1_RECT_F convert(const Rect& rect);

    Color convert(const D2D1_COLOR_F& cl);
    D2D1_COLOR_F convert(const Color& cl);
}
