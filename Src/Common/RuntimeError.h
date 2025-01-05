#pragma once

#include "Common/Precompile.h"

namespace d14engine
{
    struct RuntimeError
    {
        // The flag will be set when a RuntimeError exception is thrown.
        static bool g_flag;

        Wstring fileName = {};
        UINT lineNumber = {};

        Wstring descText = {};

        RuntimeError(WstrParam fileName, UINT lineNumber, WstrParam descText = {});

        virtual Wstring message() const;
    };

#define THROW_ERROR(Desc_Text) \
do { \
    throw RuntimeError(__FILEW__, __LINE__, Desc_Text); \
} while (0)

#define THROW_IF_TRUE(Expression) \
do { \
    if (Expression) \
    { \
        THROW_ERROR(L"Unexpected FALSE encountered: " L#Expression); \
    } \
} while (0)

#define THROW_IF_FALSE(Expression) \
do { \
    if (!(Expression)) \
    { \
        THROW_ERROR(L"Unexpected TRUE encountered: " L#Expression); \
    } \
} while (0)

#define THROW_IF_NULL(Expression) \
do { \
    if (!(Expression)) \
    { \
        THROW_ERROR(L"Unexpected nullptr encountered: " L#Expression); \
    } \
} while (0)

}
