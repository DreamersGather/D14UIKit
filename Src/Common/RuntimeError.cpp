#include "Common/Precompile.h"

#include "Common/RuntimeError.h"

namespace d14engine
{
    bool RuntimeError::g_flag = false;

    RuntimeError::RuntimeError(WstrParam fileName, UINT lineNumber, WstrParam descText)
        : fileName(fileName), lineNumber(lineNumber), m_descText(descText) { g_flag = true; }

    const Wstring& RuntimeError::descText() const
    {
        return m_descText;
    }

    Wstring RuntimeError::message() const
    {
        return L"In file: " + fileName + L", line " + std::to_wstring(lineNumber) + L", detail: " + m_descText;
    }
}
