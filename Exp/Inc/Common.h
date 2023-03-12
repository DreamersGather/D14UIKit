#pragma once

#include <functional>
#include <memory>
#include <optional>
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
    Impl* getImpl() { return pimpl.get(); }

class DllExport _D14_UIKIT_NONCOPYABLE
{
public:
    _D14_UIKIT_NONCOPYABLE() = default;

    _D14_UIKIT_NONCOPYABLE(const _D14_UIKIT_NONCOPYABLE&) = delete;
    _D14_UIKIT_NONCOPYABLE& operator=(const _D14_UIKIT_NONCOPYABLE&) = delete;
};
