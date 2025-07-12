/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 13:19
* @version: 1.0
* @description:
********************************************************************************/
#include "ScreenPass.h"

#include "../shader/vulkan/ScreenPass/SCREEN_PASS_VERT.h"
#include "../shader/vulkan/ScreenPass/SCREEN_PASS_FRAG.h"

#if OPENGL_DESKTOP
#include "../shader/gl/ScreenPass/SCREEN_PASS_VERT.h"
#include "../shader/gl/ScreenPass/SCREEN_PASS_FRAG.h"
#else
#include "../shader/gles/ScreenPass/SCREEN_PASS_VERT.h"
#include "../shader/gles/ScreenPass/SCREEN_PASS_FRAG.h"
#endif

#include "GpuPipeline.h"

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

ScreenPass::ScreenPass(HyperGpu::GpuDevice* gpuDevice) : BasePass(gpuDevice) {
	HyperGpu::RenderEnvInfo envInfo;
	envInfo.shaderInfo = HyperGpu::ShaderInfo{
		.pSpvVertexCode	   = SCREEN_PASS_VERT_SPV.data(),
		.spvVertexCodeSize = SCREEN_PASS_VERT_SPV.size(),
		.pSpvFragCode	   = SCREEN_PASS_FRAG_SPV.data(),
		.spvFragCodeSize = SCREEN_PASS_FRAG_SPV.size(),

		.pGlVertexCode = SCREEN_PASS_VERT_STR,
		.pGlFragCode = SCREEN_PASS_FRAG_STR,
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
			.loadOp = HyperGpu::AttachmentLoadOp::LOAD,
			.storeOp = HyperGpu::AttachmentStoreOp::STORE,
		}
	};
	m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateRenderPipeline(envInfo);

	HyperGpu::InputAssemblerInfo inputAssemblerInfo{
		.attributeCount = TO_U32(gVertexAttributes.size()),
		.pAttributes = gVertexAttributes.data(),
		.pVertexData = gVertexData.data(),
		.vertexSize = TO_U32(gVertexData.size() * sizeof(Vertex)),
		.vertexCount = TO_U32(gVertexData.size()),
	};
	m_pInputAssembler = m_pGpuDevice->GetResourceManager()->CreateInputAssembler(inputAssemblerInfo);
}

ScreenPass::~ScreenPass() {
}

void ScreenPass::SetScreenTexture(HyperGpu::Image2D* screenTexture) {
	this->UpdateImageBinding("screenTex", screenTexture);
}

USING_RENDER_NAMESPACE_END