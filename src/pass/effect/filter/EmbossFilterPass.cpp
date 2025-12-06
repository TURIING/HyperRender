//
// Created by turiing on 2025/12/5.
//

#include "EmbossFilterPass.h"

#include "../../../shader/vulkan/EmbossFilterPass/EMBOSS_FILTER_PASS_VERT.h"
#include "../../../shader/vulkan/EmbossFilterPass/EMBOSS_FILTER_PASS_FRAG.h"

#if OPENGL_DESKTOP
#include "../../../shader/gl/EmbossFilterPass/EMBOSS_FILTER_PASS_VERT.h"
#include "../../../shader/gl/EmbossFilterPass/EMBOSS_FILTER_PASS_FRAG.h"
#else
#include "../../../shader/gles/EmbossFilterPass/EMBOSS_FILTER_PASS_VERT.h"
#include "../../../shader/gles/EmbossFilterPass/EMBOSS_FILTER_PASS_FRAG.h"
#endif

#include "EmbossFilterPass.h"

#include "../../../common/GpuHelper.h"

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

EmbossFilterPass::EmbossFilterPass(HyperGpu::GpuDevice *pDevice): BasePass(pDevice) {
    HyperGpu::RenderEnvInfo envInfo;
    envInfo.shaderInfo = HyperGpu::ShaderInfo{
        .pSpvVertexCode	   = EMBOSS_FILTER_PASS_VERT_SPV.data(),
        .spvVertexCodeSize = EMBOSS_FILTER_PASS_VERT_SPV.size(),
        .pSpvFragCode	   = EMBOSS_FILTER_PASS_FRAG_SPV.data(),
        .spvFragCodeSize = EMBOSS_FILTER_PASS_FRAG_SPV.size(),

        .pGlVertexCode = EMBOSS_FILTER_PASS_VERT_STR,
        .pGlFragCode = EMBOSS_FILTER_PASS_FRAG_STR,
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
            .loadOp = HyperGpu::AttachmentLoadOp::DONT_CARE,
            .storeOp = HyperGpu::AttachmentStoreOp::STORE,
        }
    };
    envInfo.objName = "EmbossFilterPass";
    m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateGraphicPipeline(envInfo);

    HyperGpu::InputAssemblerInfo inputAssemblerInfo;
    inputAssemblerInfo.attributeCount = TO_U32(gVertexAttributes.size()),
    inputAssemblerInfo.pAttributes = gVertexAttributes.data(),
    inputAssemblerInfo.pVertexData = gVertexData.data(),
    inputAssemblerInfo.vertexSize = TO_U32(gVertexData.size() * sizeof(Vertex)),
    inputAssemblerInfo.vertexCount = TO_U32(gVertexData.size()),
    m_pInputAssembler = m_pGpuDevice->GetResourceManager()->CreateInputAssembler(inputAssemblerInfo);

    m_pLocalInfoBuffer = GpuHelper::CreateUniformBuffer(m_pGpuDevice, sizeof(LocalInfo));
}

EmbossFilterPass::~EmbossFilterPass() {
    m_pLocalInfoBuffer->SubRef();
}

void EmbossFilterPass::SetTargetImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("uTex", pImage);
}

void EmbossFilterPass::SetLocalInfo(float azimuth, float elevation, float bumpHeight, bool useGray) {
    LocalInfo info{};
    info.azimuth = glm::radians(azimuth);
    info.elevation = glm::radians(elevation);
    info.bumpHeight = bumpHeight;
    info.useGray = useGray ? 1 : 0;
    m_pLocalInfoBuffer->WriteData(&info, sizeof(LocalInfo));
    UpdateBufferBinding("localInfo", m_pLocalInfoBuffer);
}

USING_RENDER_NAMESPACE_END
