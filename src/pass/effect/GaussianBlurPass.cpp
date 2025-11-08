//
// Created by turiing on 2025/9/13.
//

#include "GaussianBlurPass.h"

#include "../../shader/vulkan/GaussianBlurPass/GAUSSIAN_BLUR_PASS_COMP.h"

#if OPENGL_DESKTOP
#include "../../shader/gl/GaussianBlurPass/GAUSSIAN_BLUR_PASS_COMP.h"
#else
#include "../../shader/gles/GaussianBlurPass/GAUSSIAN_BLUR_PASS_COMP.h"
#endif

#include "../../common/GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN

GaussianBlurPass::GaussianBlurPass(HyperGpu::GpuDevice *gpuDevice): BasePass(gpuDevice) {
    HyperGpu::ComputeEnvInfo computeEnvInfo;
    computeEnvInfo.shaderInfo.pSpvCompCode = GAUSSIAN_BLUR_PASS_COMP_SPV.data();
    computeEnvInfo.shaderInfo.spvCompCodeSize = GAUSSIAN_BLUR_PASS_COMP_SPV.size();
    computeEnvInfo.shaderInfo.pGlCompCode = GAUSSIAN_BLUR_PASS_COMP_STR;

    m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateComputePipeline(computeEnvInfo);
}

GaussianBlurPass::~GaussianBlurPass() = default;

void GaussianBlurPass::SetTargetImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("inputImage", pImage);
}

void GaussianBlurPass::SetOutputImage(HyperGpu::Image2D *pImage) {
    UpdateImageBinding("outputImage", pImage);
}

USING_RENDER_NAMESPACE_END
