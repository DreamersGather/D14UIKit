#pragma once

#include "Common/Precompile.h"

namespace d14engine
{
    // Usage: Declare a struct inherited from PasskeyIdiom, and append a
    // parameter of Token at the end of the ctor you want to privatize.
    // 
    // This allows the ctor behaves as private while declaring its access
    // level as public, and it is helpful when using some external utils.
    // 
    // For example, if you have an instance managed by std::unique_ptr, and
    // want to make its ctor private, in which case it is impossible to use
    // std::make_unique in friends since std::make_unique requires the ctor
    // to be public.  Use PasskeyIdiom will easily solve this problem.
    // 
    // Maybe you can escape std::make_unique with std::unique_ptr(new T),
    // but we do not want any "new" in the code.  Just use PasskeyIdiom.

    template<typename T>
    struct PasskeyIdiom
    {
        friend T;

    protected:
        struct Token { };
    };
}
