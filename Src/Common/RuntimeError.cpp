#include "Common/Precompile.h"

#include "Common/RuntimeError.h"

namespace d14engine
{
    bool RuntimeError::g_flag = false;

    RuntimeError::RuntimeError(WstrParam fileName, UINT lineNumber, WstrParam descText)
        : fileName(fileName), lineNumber(lineNumber), descText(descText) { g_flag = true; }

    Wstring RuntimeError::message() const
    {
        return L"In file: " + fileName + L", line " + std::to_wstring(lineNumber) + L", detail: " + descText;
    }
}
