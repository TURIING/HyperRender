/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 13:19
* @version: 1.0
* @description:
********************************************************************************/
#include "ScreenPass.h"

#include <GpuDevice.h>
#include "../shader/vulkan/ScreenPass/SCREEN_PASS_VERT.h"
#include "../shader/vulkan/ScreenPass/SCREEN_PASS_FRAG.h"
#include "GpuPipeline.h"

ScreenPass::ScreenPass(GpuDevice* gpuDevice) : m_pGpuDevice(gpuDevice) {
	m_pGpuDevice->AddRef();

	RenderEnvInfo envInfo;
	envInfo.shaderInfo = HyperGpu::ShaderInfo{
		.spvVertexCodeSize = SCREEN_PASS_VERT.size(),
		.pSpvVertexCode	   = SCREEN_PASS_VERT.data(),
		.spvFragCodeSize = SCREEN_PASS_FRAG.size(),
		.pSpvFragCode	   = SCREEN_PASS_FRAG.data(),
	};
	envInfo.rasterInfo = HyperGpu::RasterizationInfo{
		.primitiveType = HyperGpu::PrimitiveType::TRIANGLE,
		.frontFace	   = HyperGpu::FrontFace::CLOCK_WISE,
		.cullMode	   = HyperGpu::CullMode::BACK,
		.polygonMode   = HyperGpu::PolygonMode::FILL,
	};
	envInfo.attachments = {HyperGpu::AttachmentInfo{HyperGpu::AttachmentType::COLOR, 0}};
	m_pPipeline			= m_pGpuDevice->GetPipelineManager()->CreateRenderPipeline(envInfo);

	m_pVertexBuffer = m_pGpuDevice->GetResourceManager()->CreateBuffer(Buffer::Vertex, reinterpret_cast<uint8_t*>(m_vertexData.data()), m_vertexData.size() * sizeof(float));
}

ScreenPass::~ScreenPass() {
	GpuResourceManager::DestroyBuffer(m_pVertexBuffer);
	PipelineManager::DestroyPipeline(m_pPipeline);
	GpuFactory::DestroyDevice(m_pGpuDevice);
}
