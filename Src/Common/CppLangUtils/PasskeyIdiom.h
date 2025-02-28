#pragma once

#include "Common/Precompile.h"

namespace d14engine
{
    // Usage: Declare a struct inherited from PasskeyIdiom, and append a
    // parameter of Token at the end of the ctor you want to privatize:
    //
    // struct A, B, C;
    //
    // struct A : PasskeyIdiom<B>
    // {
    //     friend B;
    //     A(Token) { }
    //
    // private: A() { }
    // };
    //
    // struct B
    // {
    //     void func1() { A a(A::Token); } --> OK
    //     void func2() { auto a = std::make_unique<A>(); } --> Error
    //     void func3() { auto a = std::unique_ptr<A>(new A); } --> OK
    //     void func4() { auto a = std::make_unique<A>(Token); } --> OK
    // };
    //
    // struct C
    // {
    //     void func1() { A a(A::Token); } --> Error
    // };
    //
    // This allows the ctor behaves as private while declaring its access
    // level as public, and it is helpful when using some external utils.
    //
    // For example, if you have an instance managed by std::unique_ptr, and
    // want to make its ctor private, in which case it is impossible to use
    // std::make_unique in friends since std::make_unique requires the ctor
    // to be public. Using PasskeyIdiom will easily solve this problem.
    //
    // Maybe you can escape std::make_unique with std::unique_ptr(new T),
    // but we do not want any "new" in the code. Just use PasskeyIdiom.

    template<typename T>
    struct PasskeyIdiom
    {
        friend T;

    protected:
        struct Token { };
    };
}
