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

	GpuResourceManager::BufferCreateInfo vertexBufferInfo = {
		.bufferType = Buffer::Vertex,
		.bufferSize = m_vertexData.size() * sizeof(float),
		.data = reinterpret_cast<uint8_t*>(m_vertexData.data()),
	};
	GpuResourceManager::BufferCreateInfo indexBufferInfo = {
		.bufferType = Buffer::Index,
		.bufferSize = m_indices.size() * sizeof(uint32_t),
		.data = reinterpret_cast<uint8_t*>(m_indices.data()),
	};

	m_pVertexBuffer  = m_pGpuDevice->GetResourceManager()->CreateBuffer(vertexBufferInfo);
	m_pIndexBuffer   = m_pGpuDevice->GetResourceManager()->CreateBuffer(indexBufferInfo);
	m_inputAssembler = {
		.vertexBuffer = m_pVertexBuffer,
		.indexBuffer = m_pIndexBuffer,
		.vertexCount = 4,
		.indexCount = 6,
	};
}

ScreenPass::~ScreenPass() {
	GpuResourceManager::DestroyBuffer(m_pVertexBuffer);
	PipelineManager::DestroyPipeline(m_pPipeline);
	GpuFactory::DestroyDevice(m_pGpuDevice);
}

void ScreenPass::SetScreenTexture(Image2D* screenTexture) {
	m_pScreenTexture = screenTexture;
	m_pScreenTexture->AddRef();
	m_bImageDirty = true;
}

void ScreenPass::UpdateResource() {
	if (m_bImageDirty) {
		std::vector<Pipeline::ImageBindingInfo> bindingInfos;
		bindingInfos.push_back({m_pScreenTexture, 1});
		m_pPipeline->SetImages(bindingInfos.data(), bindingInfos.size());
		m_bImageDirty = false;
	}
}
