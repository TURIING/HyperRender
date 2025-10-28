//
// Created by TURIING on 2025/10/27.
//

#include "StrokePass.h"

#include "../shader/vulkan/StrokePass/STROKE_PASS_COMP.h"

#if OPENGL_DESKTOP
#include "../shader/gl/StrokePass/STROKE_PASS_COMP.h"
#else
#include "../shader/gles/StrokePass/STROKE_PASS_COMP.h"
#endif

#include "../common/GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN

StrokePass::StrokePass(HyperGpu::GpuDevice* pDevice) : BasePass(pDevice) {
	HyperGpu::ComputeEnvInfo info;
	info.shaderInfo.pGlCompCode		= STROKE_PASS_COMP_STR;
	info.shaderInfo.pSpvCompCode	= STROKE_PASS_COMP_SPV.data();
	info.shaderInfo.spvCompCodeSize = STROKE_PASS_COMP_SPV.size();

	m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateComputePipeline(info);
	m_pLocalInfoBuffer = GpuHelper::CreateUniformBuffer(m_pGpuDevice, sizeof(LocalInfo));
}

void StrokePass::SetOutputImage(HyperGpu::Image2D* image) {
	UpdateImageBinding("outputImage", image);

	m_localInfo.width  = image->GetSize().width;
	m_localInfo.height = image->GetSize().height;
	m_dirty			   = true;
}

void StrokePass::SetStrokeInfo(const std::vector<Stroke>& vecStroke, const std::vector<Circle>& vecCircle) {
	m_localInfo.numStrokes = vecStroke.size();
	m_pLocalInfoBuffer->WriteData(&m_localInfo, sizeof(LocalInfo));
	UpdateBufferBinding("localInfo", m_pLocalInfoBuffer);

	const auto sizeStroke = sizeof(Stroke) * vecStroke.size();
	m_pStrokeBuffer = GpuHelper::CreateShaderStorageBuffer(m_pGpuDevice, sizeStroke);
    void *pData = nullptr;
	m_pStrokeBuffer->Map(0, sizeStroke, &pData);
	memcpy(pData, vecStroke.data(), sizeStroke);
	m_pStrokeBuffer->UnMap();

	const auto sizeCircle = sizeof(Circle) * vecCircle.size();
	m_pCircleBuffer = GpuHelper::CreateShaderStorageBuffer(m_pGpuDevice, sizeCircle);
	m_pCircleBuffer->Map(0, sizeCircle, &pData);
	memcpy(pData, vecCircle.data(), sizeCircle);
	m_pCircleBuffer->UnMap();

	UpdateBufferBinding("strokesBuf", m_pStrokeBuffer);
	UpdateBufferBinding("circlesBuf", m_pCircleBuffer);
}

USING_RENDER_NAMESPACE_END