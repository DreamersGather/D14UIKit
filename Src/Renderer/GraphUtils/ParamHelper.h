#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer::graph_utils
{

#define NUM_ARR_ARGS(Array) _countof(Array), Array
#define ARR_NUM_ARGS(Array) Array, _countof(Array)

#define BLB_PSZ_ARGS(Blob) Blob->GetBufferPointer(), Blob->GetBufferSize()
#define BLB_SZP_ARGS(Blob) Blob->GetBufferSize(), Blob->GetBufferPointer()

}
