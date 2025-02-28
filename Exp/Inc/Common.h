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

// UI_Object from UIKit @ D14Engine
//
// _D14_UIKIT_FWDEF(UI_Object)
//
// namespace d14uikit
// {
//     class DllExport UI_Object
//     {
//         _D14_UIKIT_PIMPL(UI_Object)
//     };
// }

#define _D14_UIKIT_FWDEF(Type_Name) \
namespace d14engine::uikit { struct Type_Name; }

#define _D14_UIKIT_PIMPL(Type_Name) \
protected: \
    struct Impl; \
    std::shared_ptr<Impl> pimpl = {}; \
    explicit Type_Name(const std::shared_ptr<d14engine::uikit::Type_Name>& uiobj); \
public: \
    Impl* getImpl() const { return pimpl.get(); }

// Custom_Object from D14UIKit
//
// namespace d14uikit
// {
//     class DllExport Custom_Object
//     {
//         _D14_UIKIT_CUSTOM(Custom_Object)
//     };
// }

#define _D14_UIKIT_CUSTOM(Type_Name) \
protected: \
    struct Impl; \
    std::shared_ptr<Impl> pimpl = {}; \
    struct Passkey {}; \
    explicit Type_Name(Passkey); \
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
