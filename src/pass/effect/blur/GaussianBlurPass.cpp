//
// Created by turiing on 2025/9/13.
//

#include "GaussianBlurPass.h"

#include "../../../shader/vulkan/GaussianBlurPass/GAUSSIAN_BLUR_PASS_VERT.h"
#include "../../../shader/vulkan/GaussianBlurPass/GAUSSIAN_BLUR_PASS_FRAG.h"

#if OPENGL_DESKTOP
#include "../../../shader/gl/GaussianBlurPass/GAUSSIAN_BLUR_PASS_VERT.h"
#include "../../../shader/gl/GaussianBlurPass/GAUSSIAN_BLUR_PASS_FRAG.h"
#else
#include "../../../shader/gles/GaussianBlurPass/GAUSSIAN_BLUR_PASS_VERT.h"
#include "../../../shader/gles/GaussianBlurPass/GAUSSIAN_BLUR_PASS_FRAG.h"
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

GaussianBlurPass::GaussianBlurPass(HyperGpu::GpuDevice *gpuDevice): BasePass(gpuDevice) {
    HyperGpu::RenderEnvInfo envInfo;
    envInfo.shaderInfo = HyperGpu::ShaderInfo{
        .pSpvVertexCode	   = GAUSSIAN_BLUR_PASS_VERT_SPV.data(),
        .spvVertexCodeSize = GAUSSIAN_BLUR_PASS_VERT_SPV.size(),
        .pSpvFragCode	   = GAUSSIAN_BLUR_PASS_FRAG_SPV.data(),
        .spvFragCodeSize = GAUSSIAN_BLUR_PASS_FRAG_SPV.size(),

        .pGlVertexCode = GAUSSIAN_BLUR_PASS_VERT_STR,
        .pGlFragCode = GAUSSIAN_BLUR_PASS_FRAG_STR,
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
    envInfo.objName = "GaussianBlurPass";
    m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateGraphicPipeline(envInfo);

    HyperGpu::InputAssemblerInfo inputAssemblerInfo;
    inputAssemblerInfo.attributeCount = TO_U32(gVertexAttributes.size()),
    inputAssemblerInfo.pAttributes = gVertexAttributes.data(),
    inputAssemblerInfo.pVertexData = gVertexData.data(),
    inputAssemblerInfo.vertexSize = TO_U32(gVertexData.size() * sizeof(Vertex)),
    inputAssemblerInfo.vertexCount = TO_U32(gVertexData.size()),
    m_pInputAssembler = m_pGpuDevice->GetResourceManager()->CreateInputAssembler(inputAssemblerInfo);

    m_pLocalInfoHorizontalBuffer = GpuHelper::CreateUniformBuffer(m_pGpuDevice, sizeof(LocalInfo));
    m_pLocalInfoVerticalBuffer = GpuHelper::CreateUniformBuffer(m_pGpuDevice, sizeof(LocalInfo));
}

GaussianBlurPass::~GaussianBlurPass() {
    m_pLocalInfoHorizontalBuffer->SubRef();
    m_pLocalInfoVerticalBuffer->SubRef();
};

void GaussianBlurPass::SetTargetImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("targetTex", pImage);
}

void GaussianBlurPass::SetHorizontalLocalInfo(float radius, bool direction) {
    LocalInfo info{};
    info.radius = radius;
    info.direction = direction ? 1 : 0;
    m_pLocalInfoHorizontalBuffer->WriteData(&info, sizeof(LocalInfo));
    UpdateBufferBinding("localInfo", m_pLocalInfoHorizontalBuffer);
}

void GaussianBlurPass::SetVerticalLocalInfo(float radius, bool direction) {
    LocalInfo info{};
    info.radius = radius;
    info.direction = direction ? 1 : 0;
    m_pLocalInfoVerticalBuffer->WriteData(&info, sizeof(LocalInfo));
    UpdateBufferBinding("localInfo", m_pLocalInfoVerticalBuffer);
}

USING_RENDER_NAMESPACE_END
