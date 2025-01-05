#include "Common/Precompile.h"

#include "Renderer/GraphUtils/Texture.h"

#include "Common/DirectXError.h"

#include "Renderer/GpuBuffer.h"
#include "Renderer/GraphUtils/Bitmap.h"
#include "Renderer/Renderer.h"

namespace d14engine::renderer::graph_utils
{
    namespace texture
    {
        SharedPtr<DefaultBuffer> load(Renderer* rndr, IWICBitmapSource* source)
        {
            auto data = graph_utils::bitmap::map(source);

            UINT size = 0;
            BYTE* ptr = nullptr;
            THROW_IF_FAILED(data->GetDataPointer(&size, &ptr));

            auto texture = std::make_unique<DefaultBuffer>(rndr->d3d12Device(), size);
            texture->uploadData(rndr->cmdList(), ptr, size);

            return texture;
        }
    }
}
