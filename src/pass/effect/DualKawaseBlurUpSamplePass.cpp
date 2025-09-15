//
// Created by turiing on 2025/8/20.
//

#include "DualKawaseBlurUpSamplePass.h"


#include "../../shader/vulkan/DualKawaseBlurUpSamplePass/DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_FRAG.h"
#include "../../shader/vulkan/DualKawaseBlurUpSamplePass/DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_VERT.h"

#if OPENGL_DESKTOP
#include "../../shader/gl/DualKawaseBlurUpSamplePass/DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_VERT.h"
#include "../../shader/gl/DualKawaseBlurUpSamplePass/DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_FRAG.h"
#else
#include "../../shader/gles/DualKawaseBlurUpSamplePass/DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_VERT.h"
#include "../../shader/gles/DualKawaseBlurUpSamplePass/DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_FRAG.h"
#endif

#include "GpuPipeline.h"
#include "../../common/GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN
struct Vertex {
	glm::vec2 vPos;
};

static std::vector<HyperGpu::VertexAttribute> gVertexAttributes = {
	{ 0, HyperGpu::AttributeDataType::Vec2 },
};

static std::vector<Vertex> gVertexData = {
	// 位置             // 纹理坐标
	{{-1.0f, -1.0f}}, // 左下角
	{{1.0f, -1.0f}},  // 右下角
	{{-1.0, 1.0f}},   // 左上角
	{{1.0f, 1.0f}},   // 右上角
};

DualKawaseBlurUpSamplePass::DualKawaseBlurUpSamplePass(HyperGpu::GpuDevice* gpuDevice) : BasePass(gpuDevice) {
	HyperGpu::RenderEnvInfo envInfo;
	envInfo.shaderInfo = HyperGpu::ShaderInfo{
		.pSpvVertexCode	   = DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_VERT_SPV.data(),
		.spvVertexCodeSize = DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_VERT_SPV.size(),
		.pSpvFragCode	   = DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_FRAG_SPV.data(),
		.spvFragCodeSize = DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_FRAG_SPV.size(),

		.pGlVertexCode = DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_VERT_STR,
		.pGlFragCode = DUAL_KAWASE_BLUR_UP_SAMPLE_PASS_FRAG_STR,
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
			.loadOp = HyperGpu::AttachmentLoadOp::DONT_CARE,
			.storeOp = HyperGpu::AttachmentStoreOp::STORE,
		}
	};
	m_pPipeline = m_pGpuDevice->GetPipelineManager()->CreateGraphicPipeline(envInfo);
	m_pLocalInfoBuffer0 = GpuHelper::CreateUniformBuffer(m_pGpuDevice, sizeof(LocalInfo));
	m_pLocalInfoBuffer1 = GpuHelper::CreateUniformBuffer(m_pGpuDevice, sizeof(LocalInfo));
	m_pLocalInfoBuffer2 = GpuHelper::CreateUniformBuffer(m_pGpuDevice, sizeof(LocalInfo));

	HyperGpu::InputAssemblerInfo inputAssemblerInfo;
	inputAssemblerInfo.attributeCount = TO_U32(gVertexAttributes.size()),
	inputAssemblerInfo.pAttributes = gVertexAttributes.data(),
	inputAssemblerInfo.pVertexData = gVertexData.data(),
	inputAssemblerInfo.vertexSize = TO_U32(gVertexData.size() * sizeof(Vertex)),
	inputAssemblerInfo.vertexCount = TO_U32(gVertexData.size()),
	m_pInputAssembler = m_pGpuDevice->GetResourceManager()->CreateInputAssembler(inputAssemblerInfo);
}

DualKawaseBlurUpSamplePass::~DualKawaseBlurUpSamplePass() {
	m_pLocalInfoBuffer0->SubRef();
	m_pLocalInfoBuffer1->SubRef();
	m_pLocalInfoBuffer2->SubRef();
}

void DualKawaseBlurUpSamplePass::SetTargetTexture(HyperGpu::Image2D *targetTexture) {
	UpdateImageBinding("texture0", targetTexture);
}

void DualKawaseBlurUpSamplePass::Draw(HyperGpu::GpuCmd *pCmd, int order) {
	if (m_isLocalInfoDirty) {
		if (order == 0) {
			m_pLocalInfoBuffer0->WriteData(&m_localInfo, sizeof(LocalInfo));
			UpdateBufferBinding("localInfo", m_pLocalInfoBuffer0);
		}
		else if (order == 1) {
			m_pLocalInfoBuffer1->WriteData(&m_localInfo, sizeof(LocalInfo));
			UpdateBufferBinding("localInfo", m_pLocalInfoBuffer1);
		}
		else {
			m_pLocalInfoBuffer2->WriteData(&m_localInfo, sizeof(LocalInfo));
			UpdateBufferBinding("localInfo", m_pLocalInfoBuffer2);
		}
		m_isLocalInfoDirty = false;
	}

	BasePass::Draw(pCmd);
}

void DualKawaseBlurUpSamplePass::SetScreenSize(const Size &size) {
	m_localInfo.iResolution = { size.width, size.height };
	m_isLocalInfoDirty = true;
}

void DualKawaseBlurUpSamplePass::SetOffset(const Offset2D &offset) {
	m_localInfo.offset = { offset.x, offset.y };
	m_isLocalInfoDirty = true;
}

USING_RENDER_NAMESPACE_END