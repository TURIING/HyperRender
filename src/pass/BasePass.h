/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 13:21
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef BASE_PASS_H
#define BASE_PASS_H

#include "Render.h"
#include "HyperGpu.h"
#include "../common/common.h"

class Camera;
class BasePass : public RenderObject {
	struct GlobalInfo {
		alignas(16) glm::mat4 view = glm::mat4(1.0f);
		alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	};

public:
	explicit	 BasePass(HyperGpu::GpuDevice* pGpuDevice);
	~BasePass() override;
	void UpdateResource();
	HyperGpu::InputAssembler* GetInputAssembler() { return &m_inputAssembler; }
	void UpdateImageBinding(const std::string &name, HyperGpu::Image2D* image);
	void UpdateBufferBinding(const std::string &name, HyperGpu::Buffer* buffer);
	void InsertImageBinding(const std::string &name, HyperGpu::Image2D* image, uint32_t binding);
	void InsertBufferBinding(const std::string &name, HyperGpu::Buffer* buffer);
	void SetVertexBuffer(uint32_t vertexCount, uint64_t bufferSize, uint8_t* data);
	void SetIndexBuffer(uint32_t indexCount, uint64_t bufferSize, uint8_t* data);

protected:
	HyperGpu::GpuDevice*	 m_pGpuDevice = nullptr;
    HyperGpu::Pipeline*      m_pPipeline     = nullptr;

private:
	HyperGpu::InputAssembler m_inputAssembler;
    HyperGpu::Buffer*        m_pVertexBuffer = nullptr;
    HyperGpu::Buffer*        m_pIndexBuffer  = nullptr;
	std::unordered_map<std::string, HyperGpu::Pipeline::ImageBindingInfo> m_mapImageBinding;
	std::unordered_map<std::string, HyperGpu::Buffer*> m_mapBufferBinding;
	bool m_isImageBindingDirty = true;
	bool m_isBufferBindingDirty = true;
	Camera*           m_pCamera = nullptr;
	GlobalInfo        m_globalInfo;
	HyperGpu::Buffer* m_pGlobalBuffer = nullptr;
};

#endif //BASE_PASS_H
