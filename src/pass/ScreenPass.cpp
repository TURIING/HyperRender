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

#include "ScreenPass.h"

#include "GpuPipeline.h"
#include "../common/GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN
struct Vertex {
	glm::vec2 vPos;
	glm::vec2 vTexCoord;
};

static std::vector<HyperGpu::VertexAttribute> gVertexAttributes = {
	{ 0, HyperGpu::AttributeDataType::Vec2 },
	{1, HyperGpu::AttributeDataType::Vec2 },
};

static std::vector<HyperGpu::VertexAttribute> gInstanceAttributes = {
	{ 2, HyperGpu::AttributeDataType::Vec2 },
	{ 3, HyperGpu::AttributeDataType::Vec2 },
	{ 4, HyperGpu::AttributeDataType::Int },
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
			.format = HyperGpu::PixelFormat::R8G8B8A8_SRGB,
			.loadOp = HyperGpu::AttachmentLoadOp::CLEAR,
			.storeOp = HyperGpu::AttachmentStoreOp::STORE,
		}
	};
	m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateGraphicPipeline(envInfo);
	m_pLocalInfoBuffer = GpuHelper::CreateUniformBuffer(m_pGpuDevice, sizeof(LocalInfo));
}

ScreenPass::~ScreenPass() {}

void ScreenPass::AddScreenTexture(HyperGpu::Image2D* screenTexture, const Offset2D& screenPos) {
	this->UpdateImageBinding("screenTex", screenTexture);
	const auto size = screenTexture->GetSize();
	m_vecInstanceData.push_back({
		.iOffset = {screenPos.x, screenPos.y},
		.iSize = {size.width, size.height},
		.iTextureIndex = TO_I32(m_vecInstanceData.size()),
	});
}

void ScreenPass::ClearScreenTexture() {
	m_mapImage.clear();
	m_vecInstanceData.clear();
	m_pInputAssembler->SubRef();
	m_pInputAssembler = nullptr;
}

void ScreenPass::Draw(HyperGpu::GpuCmd *pCmd) {
	if (!m_pInputAssembler) {
		HyperGpu::InstanceInputAssemblerInfo inputAssemblerInfo;
		inputAssemblerInfo.attributeCount = TO_U32(gVertexAttributes.size()),
		inputAssemblerInfo.pAttributes = gVertexAttributes.data(),
		inputAssemblerInfo.pVertexData = gVertexData.data(),
		inputAssemblerInfo.vertexSize = TO_U32(gVertexData.size() * sizeof(Vertex)),
		inputAssemblerInfo.vertexCount = TO_U32(gVertexData.size()),
		inputAssemblerInfo.pInstanceAttributes = gInstanceAttributes.data(),
		inputAssemblerInfo.instanceAttributeCount = TO_U32(gInstanceAttributes.size()),
		inputAssemblerInfo.pInstanceData = m_vecInstanceData.data(),
		inputAssemblerInfo.instanceDataSize = TO_U32(m_vecInstanceData.size() * sizeof(InstanceData)),
		inputAssemblerInfo.instanceCount = TO_U32(m_vecInstanceData.size()),
		m_pInputAssembler = m_pGpuDevice->GetResourceManager()->CreateInputAssembler(inputAssemblerInfo);
	}

	if (m_isLocalInfoDirty) {
		m_pLocalInfoBuffer->WriteData(&m_localInfo, sizeof(LocalInfo));
		UpdateBufferBinding("localInfo", m_pLocalInfoBuffer);
		m_isLocalInfoDirty = false;
	}

	BasePass::Draw(pCmd);
}

void ScreenPass::SetScreenSize(const Size &size) {
	m_localInfo.screenSize = glm::vec2(size.width, size.height);
	m_isLocalInfoDirty = true;
}

USING_RENDER_NAMESPACE_END
