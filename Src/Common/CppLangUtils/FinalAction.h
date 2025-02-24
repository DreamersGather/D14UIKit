#pragma once

#include "Common/Precompile.h"

namespace d14engine::cpp_lang_utils
{
    // C++ does not support finally-mechanism at the semantic level (i.e. no
    // such keyword like "finally"), but actually it can be implemented with
    // RAII-mechanism in a very elegant way.
    // 
    // Usage: Simply create a FinalAction object at the top of current scope,
    // and the finally-code will be called automatically when it goes out of
    // the scope:
    //
    // void func()
    // {
    //     auto pNum = new int(1);
    //     auto clear = finally([&] { delete pNum; });
    // }
    // 
    // The definition order is significant since the dtors will be called in
    // the reverse order of creation, so the FinalAction object should be
    // created at first under normal circumstances.

    template<typename T>
    struct FinalAction
    {
        T clean;

        explicit FinalAction(const T& functor)
            :
            clean{ functor } { }

        virtual ~FinalAction() { clean(); }
    };

    // Helps deduce the template parameter type.
    template<typename T>
    FinalAction<T> finally(const T& functor)
    {
        return FinalAction<T>(functor);
    }

    // class Clazz
    // {
    //     int m_member;
    // 
    //     void func()
    //     {
    //         // Suppose m_member == 0 when func called.
    // 
    //         restoreFromException(m_member, aFuncThatChangesMember);
    // 
    //         // If the operation throws an exception after changing
    //         // m_member, the finally-code will restore it to 0.
    //     }
    // };

    template<typename T, typename FT>
    void restoreFromException(T& target, const FT& operation)
    {
        bool isTargetDirty = true;
        T originalTarget = target;

        auto restoreTarget = finally([&]
        {
            if (isTargetDirty) target = originalTarget;
        });
        operation();
        isTargetDirty = false;
    }
}
