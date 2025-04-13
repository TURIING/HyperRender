/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 15:45
* @version: 1.0
* @description: 
********************************************************************************/
#include "BasePass.h"
#include "../common/Camera.h"

BasePass::BasePass(HyperGpu::GpuDevice* pGpuDevice) : m_pGpuDevice(pGpuDevice) {
	m_pGpuDevice->AddRef();
	m_pGlobalBuffer = m_pGpuDevice->GetResourceManager()->CreateBuffer({
		.bufferType = HyperGpu::Buffer::Uniform,
		.bufferSize = sizeof(GlobalInfo),
		.binding = 0,
	});
	this->InsertBufferBinding("GlobalInfo", m_pGlobalBuffer);
	m_pCamera = new Camera();
	m_pCamera->SetOrtho(-1, 1, -1, 1, -1.0f, 1000.0f);
	m_globalInfo.view = m_pCamera->GetViewMatrix();
	m_globalInfo.proj = m_pCamera->GetProjectionMatrix();
	m_pGlobalBuffer->UpdateData(reinterpret_cast<uint8_t*>(&m_globalInfo), sizeof(GlobalInfo));
}

BasePass::~BasePass() {
	m_pCamera->SubRef();
	HyperGpu::GpuResourceManager::DestroyBuffer(m_pVertexBuffer);
	HyperGpu::GpuResourceManager::DestroyBuffer(m_pIndexBuffer);
	HyperGpu::PipelineManager::DestroyPipeline(m_pPipeline);
	m_pGpuDevice->SubRef();
}

void BasePass::UpdateResource() {
	if(m_isImageBindingDirty) {
		std::vector<HyperGpu::Pipeline::ImageBindingInfo> imageBindings;
		imageBindings.reserve(m_mapImageBinding.size());
		std::ranges::transform(m_mapImageBinding, std::back_inserter(imageBindings), [](auto &pair) {
			return pair.second;
		});

		m_pPipeline->SetImages(imageBindings.data(), imageBindings.size());
		m_isImageBindingDirty = false;
	}

	if(m_isBufferBindingDirty) {
		std::vector<HyperGpu::Buffer*> bufferBindings;
		bufferBindings.reserve(m_mapBufferBinding.size());
		std::ranges::transform(m_mapBufferBinding.begin(), m_mapBufferBinding.end(), std::back_inserter(bufferBindings), [](auto &pair) {
			return pair.second;
		});

		m_pPipeline->SetUniformBuffers(bufferBindings.data(), bufferBindings.size());
		m_isBufferBindingDirty = false;
	}
}

/**
 * 更新纹理绑定，不存在不会自动插入
 * @param name 唯一名称
 * @param image 纹理
 */
void BasePass::UpdateImageBinding(const std::string& name, HyperGpu::Image2D* image) {
	LOG_ASSERT(m_mapImageBinding.contains(name));

	auto &imageBinding = m_mapImageBinding[name];
	imageBinding.pImage = image;

	m_isImageBindingDirty = true;
}

/**
 * 更新uniform buffer绑定，不存在的话会自动插入
 * @param name 唯一名称
 * @param buffer uniform buffer
 */
void BasePass::UpdateBufferBinding(const std::string& name, HyperGpu::Buffer* buffer) {
	m_mapBufferBinding[name] = buffer;

	m_isBufferBindingDirty = true;
}

void BasePass::InsertImageBinding(const std::string& name, HyperGpu::Image2D* image, uint32_t binding) {
	m_mapImageBinding[name] = {image, binding};
}

void BasePass::InsertBufferBinding(const std::string& name, HyperGpu::Buffer* buffer) {
	m_mapBufferBinding[name] = buffer;
}

void BasePass::SetVertexBuffer(uint32_t vertexCount, uint64_t bufferSize, uint8_t* data){
	HyperGpu::GpuResourceManager::BufferCreateInfo vertexBufferInfo = {
		.bufferType = HyperGpu::Buffer::Vertex,
		.bufferSize = bufferSize,
		.data = data,
	};
	m_pVertexBuffer  = m_pGpuDevice->GetResourceManager()->CreateBuffer(vertexBufferInfo);

	m_inputAssembler.vertexBuffer = m_pVertexBuffer;
	m_inputAssembler.vertexCount = vertexCount;
}

void BasePass::SetIndexBuffer(uint32_t indexCount, uint64_t bufferSize, uint8_t* data){
	HyperGpu::GpuResourceManager::BufferCreateInfo indexBufferInfo = {
		.bufferType = HyperGpu::Buffer::Index,
		.bufferSize = bufferSize,
		.data = data,
	};
	m_pIndexBuffer   = m_pGpuDevice->GetResourceManager()->CreateBuffer(indexBufferInfo);

	m_inputAssembler.indexBuffer = m_pIndexBuffer;
	m_inputAssembler.indexCount = indexCount;
}