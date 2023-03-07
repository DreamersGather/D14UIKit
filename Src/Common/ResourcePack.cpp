#include "Common/Precompile.h"

#include "Common/ResourcePack.h"

#include "Common/RuntimeError.h"

namespace d14engine
{
    ResourcePack loadResource(WstrParam name, WstrParam type)
    {
        auto hModule = GetModuleHandle(L"D14UIKit");

        auto hResInfo = FindResource(hModule, name.c_str(), type.c_str());
        if (hResInfo == nullptr) // target not in project's resource file
        {
            THROW_ERROR(L"Resource not found, name: " + name + L", type: " + type);
        }
        auto hResData = LoadResource(hModule, hResInfo); THROW_IF_NULL(hResData);

        return { LockResource(hResData), SizeofResource(hModule, hResInfo) };
    }
}
