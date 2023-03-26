#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <string>

#ifdef _D14_UIKIT_EXP
#define DllExport __declspec(dllexport)
#else
#define DllExport // used as interfaces
#endif

#define _D14_UIKIT_PIMPL_DEF \
protected: \
    struct Impl; \
    struct Passkey { }; \
    std::shared_ptr<Impl> pimpl = {}; \
public: \
    Impl* getImpl() const \
    { \
        return pimpl.get(); \
    }

namespace d14uikit
{
    class DllExport NonCopyable
    {
    public:
        NonCopyable() = default;

        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };
}
