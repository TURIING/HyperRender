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

    HyperGpu::Buffer::BufferCreateInfo bufferCreateInfo{
        .bufferType = HyperGpu::Buffer::Uniform,
        .binding = 2,
        .bufferSize = sizeof(LocalInfo),
        .data = reinterpret_cast<uint8_t*>(&m_localInfo)
    };
    m_pLocalBuffer = m_pGpuDevice->GetResourceManager()->CreateBuffer(bufferCreateInfo);

    this->InsertImageBinding("targetTexture", nullptr, 1);
    this->InsertBufferBinding("localInfo", m_pLocalBuffer);
}

RoundCornerPass::~RoundCornerPass() {
}

void RoundCornerPass::SetScreenTexture(IDrawUnit* screenTexture) {
    const auto unit = dynamic_cast<DrawUnit*>(screenTexture);
    this->UpdateImageBinding("targetTexture", unit->GetImage());
}

void RoundCornerPass::UpdateSize(const HyperRender::Size& size) {
    BasePass::UpdateSize(size);
    m_localInfo.resolution = { size.width, size.height };
    m_pLocalBuffer->UpdateData(reinterpret_cast<uint8_t*>(&m_localInfo), sizeof(LocalInfo));
}

USING_RENDER_NAMESPACE_END