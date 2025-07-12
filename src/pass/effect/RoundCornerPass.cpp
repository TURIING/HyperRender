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

#if OPENGL_DESKTOP
#include "../../shader/gl/RoundCornerPass/ROUND_CORNER_PASS_VERT.h"
#include "../../shader/gl/RoundCornerPass/ROUND_CORNER_PASS_FRAG.h"
#else
#include "../../shader/gles/RoundCornerPass/ROUND_CORNER_PASS_VERT.h
#include "../../shader/gles/RoundCornerPass/ROUND_CORNER_PASS_FRAG.h"
#endif

USING_RENDER_NAMESPACE_BEGIN

struct Vertex {
    glm::vec2 aPos;
    glm::vec2 aTexCoord;
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

RoundCornerPass::RoundCornerPass(HyperGpu::GpuDevice* pGpuDevice): BasePass(pGpuDevice) {
    HyperGpu::RenderEnvInfo envInfo;
    envInfo.shaderInfo = {
        .pSpvVertexCode = ROUND_CORNER_PASS_VERT_SPV.data(),
        .spvVertexCodeSize = ROUND_CORNER_PASS_VERT_SPV.size(),
        .pSpvFragCode = ROUND_CORNER_PASS_FRAG_SPV.data(),
        .spvFragCodeSize = ROUND_CORNER_PASS_FRAG_SPV.size(),
        .pGlVertexCode = ROUND_CORNER_PASS_VERT_STR,
        .pGlFragCode = ROUND_CORNER_PASS_FRAG_STR,
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
            .format = HyperGpu::PixelFormat::R8G8B8A8
        }
    };

    m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateRenderPipeline(envInfo);

    const HyperGpu::InputAssemblerInfo inputAssemblerInfo{
        .attributeCount = TO_U32(gVertexAttributes.size()),
        .pAttributes = gVertexAttributes.data(),
        .pVertexData = gVertexData.data(),
        .vertexSize = TO_U32(gVertexData.size() * sizeof(Vertex)),
        .vertexCount = TO_U32(gVertexData.size()),
    };
    m_pInputAssembler = m_pGpuDevice->GetResourceManager()->CreateInputAssembler(inputAssemblerInfo);
}

RoundCornerPass::~RoundCornerPass() {
}

void RoundCornerPass::SetMainTexture(HyperGpu::Image2D* mainTexture) {
    this->UpdateImageBinding("mainTexture", mainTexture);
}


USING_RENDER_NAMESPACE_END