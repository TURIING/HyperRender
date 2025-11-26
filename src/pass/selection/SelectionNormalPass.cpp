//
// Created by turiing on 2025/11/24.
//

#include "SelectionNormalPass.h"
#include "../../common/GpuHelper.h"

#include "../../shader/vulkan/SelectionNormalPass/SELECTION_NORMAL_PASS_VERT.h"
#include "../../shader/vulkan/SelectionNormalPass/SELECTION_NORMAL_PASS_FRAG.h"
#if OPENGL_DESKTOP
#include "../../shader/gl/SelectionNormalPass/SELECTION_NORMAL_PASS_FRAG.h"
#include "../../shader/gl/SelectionNormalPass/SELECTION_NORMAL_PASS_VERT.h"
#else
#include "../../shader/gles/SelectionNormalPass/SELECTION_NORMAL_PASS_VERT.h"
#include "../../shader/gles/SelectionNormalPass/SELECTION_NORMAL_PASS_FRAG.h"
#endif

USING_RENDER_NAMESPACE_BEGIN
struct Vertex {
    glm::vec2 vPos;
    glm::vec2 vTexCoord;
};

static std::vector<HyperGpu::VertexAttribute> gVertexAttributes = {
    { 0, HyperGpu::AttributeDataType::Vec2 },
    {1, HyperGpu::AttributeDataType::Vec2 },
};

static std::vector<Vertex> gVertexData = {
    // 位置             // 纹理坐标
    {{-1.0f, -1.0f}, {0.0f, 0.0f}}, // 左下角
    {{1.0f, -1.0f}, {1.0f, 0.0f}},  // 右下角
    {{-1.0, 1.0f}, {0.0f, 1.0f}},   // 左上角
    {{1.0f, 1.0f}, {1.0f, 1.0f}},   // 右上角
};

SelectionNormalPass::SelectionNormalPass(HyperGpu::GpuDevice *pDevice): BasePass(pDevice) {
    HyperGpu::RenderEnvInfo envInfo;
    envInfo.shaderInfo = HyperGpu::ShaderInfo{
        .pSpvVertexCode	   = SELECTION_NORMAL_PASS_VERT_SPV.data(),
        .spvVertexCodeSize = SELECTION_NORMAL_PASS_VERT_SPV.size(),
        .pSpvFragCode	   = SELECTION_NORMAL_PASS_FRAG_SPV.data(),
        .spvFragCodeSize = SELECTION_NORMAL_PASS_FRAG_SPV.size(),

        .pGlVertexCode = SELECTION_NORMAL_PASS_VERT_STR,
        .pGlFragCode = SELECTION_NORMAL_PASS_FRAG_STR,
    };
    envInfo.rasterInfo = HyperGpu::RasterizationInfo{
        .primitiveType = HyperGpu::PrimitiveType::TRIANGLE_STRIP,
        .polygonMode   = HyperGpu::PolygonMode::FILL,
        .cullMode	   = HyperGpu::CullMode::BACK,
        .frontFace	   = HyperGpu::FrontFace::CLOCK_WISE,
    };
    envInfo.attachments = {
        {
            .type = HyperGpu::AttachmentType::COLOR,
            .index = 0,
            .format = HyperGpu::PixelFormat::R8G8B8A8_SRGB,
            .loadOp = HyperGpu::AttachmentLoadOp::LOAD,
            .storeOp = HyperGpu::AttachmentStoreOp::STORE,
        }
    };
    envInfo.objName = "SelectionNormalPass";
    m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateGraphicPipeline(envInfo);

    m_pLocalInfoBuffer = GpuHelper::CreateUniformBuffer(m_pGpuDevice, sizeof(LocalInfo));
}

void SelectionNormalPass::SetLocalInfo(const glm::vec2 &rectMin, const glm::vec2 &rectMax, const glm::vec4 &color, float density, float time) {
    LocalInfo info;
    info.color = color;
    info.density = density;
    info.rectMax = rectMax;
    info.rectMin = rectMin;
    info.time = time;
    m_pLocalInfoBuffer->WriteData(&info, sizeof(LocalInfo));
    UpdateBufferBinding("localInfo", m_pLocalInfoBuffer);
}

void SelectionNormalPass::Draw(HyperGpu::GpuCmd *pCmd) {
    if (!m_pInputAssembler) {
        HyperGpu::InputAssemblerInfo inputAssemblerInfo;
        inputAssemblerInfo.attributeCount = TO_U32(gVertexAttributes.size()),
        inputAssemblerInfo.pAttributes = gVertexAttributes.data(),
        inputAssemblerInfo.pVertexData = gVertexData.data(),
        inputAssemblerInfo.vertexSize = TO_U32(gVertexData.size() * sizeof(Vertex)),
        inputAssemblerInfo.vertexCount = TO_U32(gVertexData.size()),
        m_pInputAssembler = m_pGpuDevice->GetResourceManager()->CreateInputAssembler(inputAssemblerInfo);
    }
    BasePass::Draw(pCmd);
}

USING_RENDER_NAMESPACE_END
