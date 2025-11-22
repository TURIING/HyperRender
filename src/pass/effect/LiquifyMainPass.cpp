//
// Created by turiing on 2025/11/22.
//

#include "LiquifyMainPass.h"
#include "../../shader/vulkan/LiquifyPass/LIQUIFY_MAIN_PASS_VERT.h"
#include "../../shader/vulkan/LiquifyPass/LIQUIFY_MAIN_PASS_FRAG.h"

#if OPENGL_DESKTOP
#include "../../shader/gl/LiquifyPass/LIQUIFY_MAIN_PASS_VERT.h"
#include "../../shader/gl/LiquifyPass/LIQUIFY_MAIN_PASS_FRAG.h"
#else
#include "../../shader/gles/LiquifyPass/LIQUIFY_MAIN_PASS_VERT.h"
#include "../../shader/gles/LiquifyPass/LIQUIFY_MAIN_PASS_FRAG.h"
#endif
#include "LiquifyMainPass.h"

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

LiquifyMainPass::LiquifyMainPass(HyperGpu::GpuDevice *pGpuDevice): BasePass(pGpuDevice) {
    HyperGpu::RenderEnvInfo envInfo;
    envInfo.shaderInfo = HyperGpu::ShaderInfo{
        .pSpvVertexCode	   = LIQUIFY_MAIN_PASS_VERT_SPV.data(),
        .spvVertexCodeSize = LIQUIFY_MAIN_PASS_VERT_SPV.size(),
        .pSpvFragCode	   = LIQUIFY_MAIN_PASS_FRAG_SPV.data(),
        .spvFragCodeSize = LIQUIFY_MAIN_PASS_FRAG_SPV.size(),

        .pGlVertexCode = LIQUIFY_MAIN_PASS_VERT_STR,
        .pGlFragCode = LIQUIFY_MAIN_PASS_FRAG_STR,
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
            .format = HyperGpu::PixelFormat::R16G16B16A16_SFLOAT,
            .loadOp = HyperGpu::AttachmentLoadOp::DONT_CARE,
            .storeOp = HyperGpu::AttachmentStoreOp::STORE,
        }
    };
    envInfo.objName = "LiquifyMainPass";
    m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateGraphicPipeline(envInfo);
    m_pLocalInfoBuffer = GpuHelper::CreateUniformBuffer(pGpuDevice, sizeof(LocalInfo));
}

LiquifyMainPass::~LiquifyMainPass() = default;

void LiquifyMainPass::SetUvInputImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("uvReadTex", pImage);
}

void LiquifyMainPass::SetLocalInfo(const Size &resolution, const PointI &newMousePos, const PointI &oldMousePos, bool isPressed, bool firstFrame) {
    LocalInfo info{};
    info.resolution = glm::vec2(resolution.width, resolution.height);
    info.firstFrame = firstFrame ? 1 : 0;
    info.isPressed = isPressed ? 1 : 0;
    info.newMouse = glm::vec2(newMousePos.x, newMousePos.y);
    info.oldMouse = glm::vec2(oldMousePos.x, oldMousePos.y);
    m_pLocalInfoBuffer->WriteData(&info, sizeof(LocalInfo));
    UpdateBufferBinding("localInfo", m_pLocalInfoBuffer);
}

void LiquifyMainPass::Draw(HyperGpu::GpuCmd *pCmd) {
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
