#include "Common/Precompile.h"

#ifdef _D14_UIKIT_EXP_PY_BIND

#include "Common.h"

#include "Application.h"
#include "BasicType.h"

PYBIND11_MODULE(D14UIKit, m)
{
    initApplication(m);
    initBasicType(m);
}

#endif
