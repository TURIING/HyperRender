/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 14:39
* @version: 1.0
* @description: 
********************************************************************************/
#include "RoundCornerPass.h"
#include "../../shader/vulkan/RoundCornerPass/ROUND_CORNER_PASS_FRAG.h"
#include "../../shader/vulkan/RoundCornerPass/ROUND_CORNER_PASS_VERT.h"

USING_RENDER_NAMESPACE_BEGIN

RoundCornerPass::RoundCornerPass(HyperGpu::GpuDevice* pGpuDevice): BasePass(pGpuDevice) {
    HyperGpu::AttachmentInfo attachment[] = {
        {
            .type = HyperGpu::AttachmentType::COLOR,
            .index = 0,
            .format = HyperGpu::PixelFormat::R8G8B8A8
        }
    };

    HyperGpu::RenderEnvInfo envInfo{
        .shaderInfo = {
            .pSpvVertexCode = ROUND_CORNER_PASS_VERT.data(),
            .spvVertexCodeSize = ROUND_CORNER_PASS_VERT.size(),
            .pSpvFragCode = ROUND_CORNER_PASS_FRAG.data(),
            .spvFragCodeSize = ROUND_CORNER_PASS_FRAG.size(),
        },
        .pAttachment = attachment,
        .attachmentCount = std::size(attachment),
    };
    m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateRenderPipeline(envInfo);


    // HyperGpu::Buffer::BufferCreateInfo bufferCreateInfo{
    //     .bufferType = HyperGpu::Buffer::Uniform,
    //     .bufferSize = sizeof(LocalInfo),
    //     .data = reinterpret_cast<uint8_t*>(&m_localInfo),
    //     .binding = 2,
    // };
    // m_pLocalBuffer = m_pGpuDevice->GetResourceManager()->CreateBuffer(bufferCreateInfo);
    //
    // this->InsertImageBinding("targetTexture", nullptr, 1);
    // this->InsertBufferBinding("localInfo", m_pLocalBuffer);
}

RoundCornerPass::~RoundCornerPass() {
}

void RoundCornerPass::SetScreenTexture(IDrawUnit* screenTexture) {
    const auto unit = dynamic_cast<DrawUnit*>(screenTexture);
    this->UpdateImageBinding("targetTexture", unit->GetImage());
}


USING_RENDER_NAMESPACE_END