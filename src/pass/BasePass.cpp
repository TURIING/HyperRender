/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 15:45
* @version: 1.0
* @description: 
********************************************************************************/
#include "BasePass.h"
#include "../common/Camera.h"

USING_RENDER_NAMESPACE_BEGIN

BasePass::BasePass(HyperGpu::GpuDevice* pGpuDevice) : m_pGpuDevice(pGpuDevice) {
	m_pGpuDevice->AddRef();
}

BasePass::~BasePass() {
	m_pPipeline->SubRef();
	m_pInputAssembler->SubRef();
	m_pGpuDevice->SubRef();
}

/**
 * 更新纹理绑定，不存在会自动插入
 * @param name 唯一名称
 * @param image 纹理
 */
void BasePass::UpdateImageBinding(const std::string& name, HyperGpu::Image2D* image) {
	m_mapImage[name] = { image };
	// if (!m_mapImage.contains(name)) {
	// }
	// else {
	// 	m_mapImage[name].push_back(image);
	// }
}

/**
 * 更新uniform buffer绑定，不存在的话会自动插入
 * @param name 唯一名称
 * @param buffer uniform buffer
 */
void BasePass::UpdateBufferBinding(const std::string& name, HyperGpu::Buffer* buffer) {
	m_mapBuffer[name] = buffer;
}

void BasePass::SetGlobalUniform(HyperGpu::Buffer* pGlobalBuffer) {
	UpdateBufferBinding("globalInfo", pGlobalBuffer);
}

void BasePass::Draw(HyperGpu::GpuCmd* pCmd) {
	std::vector<HyperGpu::ImageBinding> imageBindings;
	imageBindings.reserve(m_mapImage.size());
	std::ranges::transform(m_mapImage, std::back_inserter(imageBindings), [](auto &pair) {
		return HyperGpu::ImageBinding {pair.second.data(), TO_U32(pair.second.size()), pair.first.c_str()};
	});

	std::vector<HyperGpu::UniformBinding> bufferBindings;
	bufferBindings.reserve(m_mapBuffer.size());
	std::ranges::transform(m_mapBuffer.begin(), m_mapBuffer.end(), std::back_inserter(bufferBindings), [](auto &pair) {
		return HyperGpu::UniformBinding {pair.second,  pair.first.c_str()};
	});

	HyperGpu::DrawInfo drawInfo {
		.pInputAssembler = m_pInputAssembler,
		.pUniformBinding = bufferBindings.data(),
		.uniformBindingCount = TO_U32(bufferBindings.size()),
		.pImageBinding = imageBindings.data(),
		.imageBindingCount = TO_U32(imageBindings.size()),
	};
	pCmd->Draw(drawInfo);
}

void BasePass::Dispatch(HyperGpu::GpuCmd *pCmd, uint32_t x, uint32_t y, uint32_t z) {
	std::vector<HyperGpu::ImageBinding> imageBindings;
	imageBindings.reserve(m_mapImage.size());
	std::ranges::transform(m_mapImage, std::back_inserter(imageBindings), [](auto &pair) {
		return HyperGpu::ImageBinding {pair.second.data(), TO_U32(pair.second.size()), pair.first.c_str()};
	});

	std::vector<HyperGpu::UniformBinding> bufferBindings;
	bufferBindings.reserve(m_mapBuffer.size());
	std::ranges::transform(m_mapBuffer.begin(), m_mapBuffer.end(), std::back_inserter(bufferBindings), [](auto &pair) {
		return HyperGpu::UniformBinding {pair.second,  pair.first.c_str()};
	});

	HyperGpu::DispatchInfo dispatchInfo {
		.pPipeline = m_pPipeline,
		.pUniformBinding = bufferBindings.data(),
		.uniformBindingCount = TO_U32(bufferBindings.size()),
		.pImageBinding = imageBindings.data(),
		.imageBindingCount = TO_U32(imageBindings.size()),
		.groupCountX = x,
		.groupCountY = y,
		.groupCountZ = z
	};
	pCmd->Dispatch(dispatchInfo);
}

void BasePass::SetBlendType(BlendType blendType) const {
	auto &blendInfo = m_pPipeline->renderEnvInfo.blendInfo;

	switch (blendType) {
		case BlendType::Cover: {
			blendInfo.enable = false;
			break;
		}
		case BlendType::Multiply: {
			blendInfo.enable = true;
			blendInfo.srcColorBlendFactor = HyperGpu::BlendFactor::DST_COLOR;
			blendInfo.dstColorBlendFactor = HyperGpu::BlendFactor::ZERO;
			blendInfo.srcAlphaBlendFactor = HyperGpu::BlendFactor::ONE;
			blendInfo.dstAlphaBlendFactor = HyperGpu::BlendFactor::ZERO;
			blendInfo.colorBlendOp = HyperGpu::BlendOp::ADD;
			blendInfo.alphaBlendOp = HyperGpu::BlendOp::ADD;
			break;
		}
		case BlendType::Normal: {
			blendInfo.enable = true;
			blendInfo.srcColorBlendFactor = HyperGpu::BlendFactor::SRC_ALPHA;
			blendInfo.dstColorBlendFactor = HyperGpu::BlendFactor::ONE_MINUS_SRC_ALPHA;
			blendInfo.srcAlphaBlendFactor = HyperGpu::BlendFactor::SRC_ALPHA;
			blendInfo.dstAlphaBlendFactor = HyperGpu::BlendFactor::ONE_MINUS_SRC_ALPHA;
			blendInfo.colorBlendOp = HyperGpu::BlendOp::ADD;
			blendInfo.alphaBlendOp = HyperGpu::BlendOp::ADD;
			break;
		}
		default:	LOG_ASSERT_INFO(false, "Invalid BlendType");
	}
}

USING_RENDER_NAMESPACE_END
