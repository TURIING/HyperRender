//
// Created by turiing on 2025/11/22.
//

#include "LiquifyPass.h"
#include "../../shader/vulkan/LiquifyPass/LIQUIFY_MAIN_PASS_COMP.h"

#if OPENGL_DESKTOP
#include "../../shader/gl/LiquifyPass/LIQUIFY_MAIN_PASS_COMP.h"
#else
#include "../../shader/gles/LiquifyPass/LIQUIFY_MAIN_PASS_COMP.h"
#endif
#include "LiquifyPass.h"

#include "../../common/GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN
LiquifyPass::LiquifyPass(HyperGpu::GpuDevice *pGpuDevice): BasePass(pGpuDevice) {
    HyperGpu::ComputeEnvInfo computeEnvInfo;
    computeEnvInfo.shaderInfo.pSpvCompCode = LIQUIFY_MAIN_PASS_COMP_SPV.data();
    computeEnvInfo.shaderInfo.spvCompCodeSize = LIQUIFY_MAIN_PASS_COMP_SPV.size();
    computeEnvInfo.shaderInfo.pGlCompCode = LIQUIFY_MAIN_PASS_COMP_STR;
    m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateComputePipeline(computeEnvInfo);
    m_pLocalInfoBuffer = GpuHelper::CreateUniformBuffer(pGpuDevice, sizeof(LocalInfo));
}

LiquifyPass::~LiquifyPass() = default;

void LiquifyPass::SetTargetImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("targetTex", pImage);
}

void LiquifyPass::SetUvInputImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("uvInputImage", pImage);
}

void LiquifyPass::SetUvOutputImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("uvOutputImage", pImage);
}

void LiquifyPass::SetOutputImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("outputImage", pImage);
}

void LiquifyPass::SetLocalInfo(const Size &resolution, const PointI &newMousePos, const PointI &oldMousePos, bool isPressed, bool firstFrame) {
    LocalInfo info{};
    info.resolution = glm::vec2(resolution.width, resolution.height);
    info.firstFrame = firstFrame ? 1 : 0;
    info.isPressed = isPressed ? 1 : 0;
    info.newMouse = glm::vec2(newMousePos.x, newMousePos.y);
    info.oldMouse = glm::vec2(oldMousePos.x, oldMousePos.y);
    m_pLocalInfoBuffer->WriteData(&info, sizeof(LocalInfo));
    UpdateBufferBinding("localInfo", m_pLocalInfoBuffer);
}

USING_RENDER_NAMESPACE_END
