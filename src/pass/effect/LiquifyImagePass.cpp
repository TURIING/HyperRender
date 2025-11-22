//
// Created by turiing on 2025/11/22.
//

#include "LiquifyImagePass.h"
#include "../../shader/vulkan/LiquifyPass/LIQUIFY_IMAGE_PASS_VERT.h"
#include "../../shader/vulkan/LiquifyPass/LIQUIFY_IMAGE_PASS_FRAG.h"

#if OPENGL_DESKTOP
#include "../../shader/gl/LiquifyPass/LIQUIFY_IMAGE_PASS_VERT.h"
#include "../../shader/gl/LiquifyPass/LIQUIFY_IMAGE_PASS_FRAG.h"
#else
#include "../../shader/gles/LiquifyPass/LIQUIFY_IMAGE_PASS_VERT.h"
#include "../../shader/gles/LiquifyPass/LIQUIFY_IMAGE_PASS_FRAG.h"
#endif
#include "LiquifyImagePass.h"

#include "../../common/GpuHelper.h"

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

LiquifyImagePass::LiquifyImagePass(HyperGpu::GpuDevice *pGpuDevice): BasePass(pGpuDevice) {
    HyperGpu::RenderEnvInfo envInfo;
    envInfo.shaderInfo = HyperGpu::ShaderInfo{
        .pSpvVertexCode	   = LIQUIFY_IMAGE_PASS_VERT_SPV.data(),
        .spvVertexCodeSize = LIQUIFY_IMAGE_PASS_VERT_SPV.size(),
        .pSpvFragCode	   = LIQUIFY_IMAGE_PASS_FRAG_SPV.data(),
        .spvFragCodeSize = LIQUIFY_IMAGE_PASS_FRAG_SPV.size(),

        .pGlVertexCode = LIQUIFY_IMAGE_PASS_VERT_STR,
        .pGlFragCode = LIQUIFY_IMAGE_PASS_FRAG_STR,
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
            .format = HyperGpu::PixelFormat::R8G8B8A8,
            .loadOp = HyperGpu::AttachmentLoadOp::DONT_CARE,
            .storeOp = HyperGpu::AttachmentStoreOp::STORE,
        }
    };
    envInfo.objName = "LiquifyImagePass";
    m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateGraphicPipeline(envInfo);
}

LiquifyImagePass::~LiquifyImagePass() = default;

void LiquifyImagePass::SetUvInputImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("uvReadTex", pImage);
}

void LiquifyImagePass::SetTargetImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("targetTex", pImage);
}

void LiquifyImagePass::Draw(HyperGpu::GpuCmd *pCmd) {
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
