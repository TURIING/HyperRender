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

USING_RENDER_NAMESPACE_BEGIN

ScreenPass::ScreenPass(GpuDevice* gpuDevice) : BasePass(gpuDevice) {
	HyperGpu::AttachmentInfo attachment[] = {
		{
			.type = HyperGpu::AttachmentType::COLOR,
			.index = 0,
			.format = HyperGpu::PixelFormat::R8G8B8A8
		}
	};

	RenderEnvInfo envInfo;
	envInfo.shaderInfo = HyperGpu::ShaderInfo{
		.pSpvVertexCode	   = SCREEN_PASS_VERT.data(),
		.spvVertexCodeSize = SCREEN_PASS_VERT.size(),
		.pSpvFragCode	   = SCREEN_PASS_FRAG.data(),
		.spvFragCodeSize = SCREEN_PASS_FRAG.size(),
	};
	envInfo.rasterInfo = HyperGpu::RasterizationInfo{
		.primitiveType = HyperGpu::PrimitiveType::TRIANGLE,
		.polygonMode   = HyperGpu::PolygonMode::FILL,
		.cullMode	   = HyperGpu::CullMode::BACK,
		.frontFace	   = HyperGpu::FrontFace::CLOCK_WISE,
	};
	envInfo.pAttachment     = attachment;
	envInfo.attachmentCount = std::size(attachment);
	m_pPipeline             = m_pGpuDevice->GetPipelineManager()->CreateRenderPipeline(envInfo);

	this->SetVertexBuffer(m_vertexData.size(), m_vertexData.size() * sizeof(Vertex),
	                      reinterpret_cast<uint8_t*>(m_vertexData.data()));
	this->SetIndexBuffer(m_indices.size(), m_indices.size() * sizeof(uint32_t),
	                     reinterpret_cast<uint8_t*>(m_indices.data()));
	this->InsertImageBinding("screenTexture", nullptr, 1);
}

ScreenPass::~ScreenPass() {
}

void ScreenPass::SetScreenTexture(Image2D* screenTexture) {
	this->UpdateImageBinding("screenTexture", screenTexture);
}

USING_RENDER_NAMESPACE_END