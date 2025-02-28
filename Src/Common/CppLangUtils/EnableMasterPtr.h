#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/NonCopyable.h"

namespace d14engine::cpp_lang_utils
{
    // Usage: Declare an inner struct inherited from EnableMasterPtr, and
    // access the parent with m_master:
    //
    // struct Parent
    // {
    //     struct Child : EnableMasterPtr<Parent>
    //     {
    //         using EnableMasterPtr::EnableMasterPtr;
    //         void func() { Parent* parent = m_master; }
    //     }
    //     child{ this };
    // };
    //
    // Since the parent is referenced through raw pointer, it is unsafe to
    // create an object outside the parent, so it is necessary to make the
    // ctor private and prevent any copying.

    template<typename T>
    struct EnableMasterPtr : NonCopyable
    {
        friend T;

        using EnableMasterPtrType = EnableMasterPtr<T>;

    protected:
        explicit EnableMasterPtr(T* master) : m_master(master) { }

        T* m_master = nullptr;
    };
}
