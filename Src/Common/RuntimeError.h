#pragma once

#include "Common/Precompile.h"

namespace d14engine
{
    struct RuntimeError
    {
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

#define THROW_IF_NULL(Expression) \
do { \
    if (!(Expression)) \
    { \
        THROW_ERROR(L"Unexpected nullptr encountered: " L#Expression); \
    } \
} while (0)

}
