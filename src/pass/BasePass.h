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

USING_RENDER_NAMESPACE_BEGIN

class Camera;
class BasePass : public RenderObject {

public:
	explicit BasePass(HyperGpu::GpuDevice* pGpuDevice);
	~BasePass() override;
	void UpdateImageBinding(const std::string &name, HyperGpu::Image2D* image);
	void UpdateBufferBinding(const std::string &name, HyperGpu::Buffer* buffer);
    NODISCARD HyperGpu::Pipeline* GetPipeline() const { return m_pPipeline; }
	void SetGlobalUniform(HyperGpu::Buffer* pGlobalBuffer);
	void Draw(HyperGpu::GpuCmd* pCmd);

protected:
	HyperGpu::GpuDevice* m_pGpuDevice = nullptr;
    HyperGpu::Pipeline* m_pPipeline = nullptr;

private:
	HyperGpu::InputAssembler *m_pInputAssembler = nullptr;
	std::unordered_map<std::string, HyperGpu::Image2D*> m_mapImage;
	std::unordered_map<std::string, HyperGpu::Buffer*> m_mapBuffer;
};

USING_RENDER_NAMESPACE_END
#endif //BASE_PASS_H
