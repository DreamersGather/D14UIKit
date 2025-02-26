#pragma once

#include "Common/Precompile.h"

namespace d14engine
{
    struct RuntimeError
    {
        // The flag will be set when a RuntimeError exception is thrown.
        static bool g_flag;

        const Wstring fileName = {};
        const UINT lineNumber = {};

        RuntimeError(WstrParam fileName, UINT lineNumber, WstrParam descText = {});

    protected:
        Wstring m_descText = {};

    public:
        const Wstring& descText() const;

        // Returns detailed information about the specific RuntimeError,
        // usually called in a catch block to assist with debugging.
        virtual Wstring message() const;
    };
}
#define _D14_RUNTIME_ERROR true

/////////////////
// Throw Error //
/////////////////

#if _D14_RUNTIME_ERROR
#define THROW_ERROR(Desc_Text) \
do { \
    throw RuntimeError(__FILEW__, __LINE__, Desc_Text); \
} while (0)
#else
#define THROW_ERROR(Desc_Text)
#endif

///////////////////
// Throw If True //
///////////////////

#if _D14_RUNTIME_ERROR
#define THROW_IF_TRUE(Expression) \
do { \
    if (Expression) \
    { \
        THROW_ERROR(L"Unexpected TRUE encountered: " L#Expression); \
    } \
} while (0)
#else
#define THROW_IF_TRUE(Expression) (Expression)
#endif

////////////////////
// Throw If False //
////////////////////

#if _D14_RUNTIME_ERROR
#define THROW_IF_FALSE(Expression) \
do { \
    if (!(Expression)) \
    { \
        THROW_ERROR(L"Unexpected FALSE encountered: " L#Expression); \
    } \
} while (0)
#else
#define THROW_IF_FALSE(Expression) (Expression)
#endif

///////////////////
// Throw If Null //
///////////////////

#if _D14_RUNTIME_ERROR
#define THROW_IF_NULL(Expression) \
do { \
    if (!(Expression)) \
    { \
        THROW_ERROR(L"Unexpected NULL encountered: " L#Expression); \
    } \
} while (0)
#else
#define THROW_IF_NULL(Expression) (Expression)
#endif
