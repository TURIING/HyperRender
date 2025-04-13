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

RoundCornerPass::RoundCornerPass(HyperGpu::GpuDevice* pGpuDevice): BasePass(pGpuDevice) {
    HyperGpu::AttachmentInfo attachment[] = {
        {
            .type = HyperGpu::AttachmentType::COLOR,
            .index = 0
        }
    };

    HyperGpu::RenderEnvInfo envInfo{
        .shaderInfo = {
            .spvVertexCodeSize = ROUND_CORNER_PASS_VERT.size(),
            .pSpvVertexCode = ROUND_CORNER_PASS_VERT.data(),
            .spvFragCodeSize = ROUND_CORNER_PASS_FRAG.size(),
            .pSpvFragCode = ROUND_CORNER_PASS_FRAG.data()
        },
        .pAttachment = attachment,
        .attachmentCount = std::size(attachment),
    };
    m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateRenderPipeline(envInfo);

    this->SetVertexBuffer(m_vertexData.size(), m_vertexData.size() * sizeof(Vertex),
                          reinterpret_cast<uint8_t*>(m_vertexData.data()));
    this->SetIndexBuffer(m_indices.size(), m_indices.size() * sizeof(uint32_t),
                         reinterpret_cast<uint8_t*>(m_indices.data()));

    HyperGpu::GpuResourceManager::BufferCreateInfo bufferCreateInfo{
        .bufferType = HyperGpu::Buffer::Uniform,
        .binding = 0,
        .bufferSize = sizeof(LocalInfo),
        .data = reinterpret_cast<uint8_t*>(&m_localInfo)
    };
    m_pLocalBuffer = m_pGpuDevice->GetResourceManager()->CreateBuffer(bufferCreateInfo);

    this->InsertImageBinding("targetTexture", nullptr, 1);
    this->InsertBufferBinding("localInfo", m_pLocalBuffer);
}

RoundCornerPass::~RoundCornerPass() {
}

void RoundCornerPass::SetScreenTexture(HyperGpu::Image2D* screenTexture) {
    this->UpdateImageBinding("targetTexture", screenTexture);
}
