#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <string>

// Allows the macro expansion substitutes
// empty texts for the missing parameters.
#pragma warning(disable : 4003)

#ifdef _D14_UIKIT_EXP
#define DllExport __declspec(dllexport)
#else
#define DllExport // used as interfaces
#endif

#define _D14_UIKIT_PIMPL(Type_Name) \
protected: \
    struct Impl; \
    std::shared_ptr<Impl> pimpl = {}; \
    struct Passkey { }; \
    explicit Type_Name(Passkey); \
    void initialize(); \
public: \
    Impl* getImpl() const { return pimpl.get(); }

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
