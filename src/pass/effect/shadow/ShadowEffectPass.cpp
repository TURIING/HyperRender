//
// Created by turiing on 2025/12/7.
//

#include "ShadowEffectPass.h"
#include "../../../shader/vulkan/common/COMMON_RECTANGLE_PASS_VERT.h"
#include "../../../shader/vulkan/ShadowEffectPass/SHADOW_EFFECT_PASS_FRAG.h"

#if OPENGL_DESKTOP
#include "../../../shader/gl/common/COMMON_RECTANGLE_PASS_VERT.h"
#include "../../../shader/gl/ShadowEffectPass/SHADOW_EFFECT_PASS_FRAG.h"
#else
#include "../../../shader/gles/common/COMMON_RECTANGLE_PASS_VERT.h"
#include "../../../shader/gles/ShadowEffectPass/SHADOW_EFFECT_PASS_FRAG.h"
#endif

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

ShadowEffectPass::ShadowEffectPass(HyperGpu::GpuDevice *pGpuDevice): BasePass(pGpuDevice) {
    HyperGpu::RenderEnvInfo envInfo;
    envInfo.shaderInfo = HyperGpu::ShaderInfo{
        .pSpvVertexCode	   = COMMON_RECTANGLE_PASS_VERT_SPV.data(),
        .spvVertexCodeSize = COMMON_RECTANGLE_PASS_VERT_SPV.size(),
        .pSpvFragCode	   = SHADOW_EFFECT_PASS_FRAG_SPV.data(),
        .spvFragCodeSize = SHADOW_EFFECT_PASS_FRAG_SPV.size(),

        .pGlVertexCode = COMMON_RECTANGLE_PASS_VERT_STR,
        .pGlFragCode = SHADOW_EFFECT_PASS_FRAG_STR,
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
    envInfo.objName = "ShadowEffectPass";
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

ShadowEffectPass::~ShadowEffectPass() {
    m_pLocalInfoBuffer->SubRef();
}

void ShadowEffectPass::SetLocalInfo(const Offset2D &offset) {
    LocalInfo info{};
    info.offset = { offset.x, offset.y };
    m_pLocalInfoBuffer->WriteData(&info, sizeof(LocalInfo));
    UpdateBufferBinding("localInfo", m_pLocalInfoBuffer);
}

void ShadowEffectPass::SetTargetImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("targetTex", pImage);
}

void ShadowEffectPass::SetAlphaMask(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("alphaTex", pImage);
}

USING_RENDER_NAMESPACE_END
