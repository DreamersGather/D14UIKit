#include "Common/Precompile.h"

#include "Common/DirectXError.h"

namespace d14engine
{
    DirectXError::DirectXError(HRESULT hr, WstrParam fileName, UINT lineNumber)
        : RuntimeError(fileName, lineNumber), hr(hr) { descText = _com_error(hr).ErrorMessage(); }
}
