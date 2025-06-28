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
	m_mapImage[name] = image;
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
	UpdateBufferBinding("GlobalInfo", pGlobalBuffer);
}

void BasePass::Draw(HyperGpu::GpuCmd* pCmd) {
	std::vector<HyperGpu::GpuCmd::ImageBinding> imageBindings;
	imageBindings.reserve(m_mapImage.size());
	std::ranges::transform(m_mapImage, std::back_inserter(imageBindings), [](auto &pair) {
		return HyperGpu::GpuCmd::ImageBinding {pair.second, pair.first.c_str()};
	});

	std::vector<HyperGpu::GpuCmd::UniformBinding> bufferBindings;
	bufferBindings.reserve(m_mapBuffer.size());
	std::ranges::transform(m_mapBuffer.begin(), m_mapBuffer.end(), std::back_inserter(bufferBindings), [](auto &pair) {
		return HyperGpu::GpuCmd::UniformBinding {pair.second,  pair.first.c_str()};
	});

	HyperGpu::GpuCmd::DrawInfo drawInfo {
		.pInputAssembler = m_pInputAssembler,
		.pUniformBinding = bufferBindings.data(),
		.uniformBindingCount = TO_U32(bufferBindings.size()),
		.pImageBinding = imageBindings.data(),
		.imageBindingCount = TO_U32(imageBindings.size()),
	};
	pCmd->Draw(drawInfo);
}

USING_RENDER_NAMESPACE_END