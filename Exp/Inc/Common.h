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
