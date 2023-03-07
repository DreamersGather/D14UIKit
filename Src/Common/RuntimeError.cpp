#include "Common/Precompile.h"

#include "Common/RuntimeError.h"

namespace d14engine
{
    RuntimeError::RuntimeError(WstrParam fileName, UINT lineNumber, WstrParam descText)
        : fileName(fileName), lineNumber(lineNumber), descText(descText) { }

    Wstring RuntimeError::message() const
    {
        return L"In file: " + fileName + L", line " + std::to_wstring(lineNumber) + L", detail: " + descText;
    }
}
