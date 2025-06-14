/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 15:29
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef BASETOOL_H
#define BASETOOL_H

#include "../../include/ITool.h"
#include "../common/common.h"
#include "HyperGpu.h"

USING_RENDER_NAMESPACE_BEGIN

class BaseTool : virtual public ITool {
	struct GlobalInfo {
		alignas(16) glm::mat4 view = glm::mat4(1.0f);
		alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	};

public:
	explicit BaseTool(HyperGpu::GpuDevice* pGpuDevice);
	~BaseTool() override;
	void UpdateSize(const Size &size);
	void ClearColor(IDrawUnit *targetUnit, Color color) override;
	NODISCARD DrawUnit *CreateDrawUnit(const Area &area) override;

protected:
	void clearColor(HyperGpu::GpuCmd *pCmd, IDrawUnit *targetUnit, Color color);
	void begin() const;
	void end(const std::vector<HyperGpu::Semaphore *> &signalSemaphore) const;
	void SubmitCmd(const std::vector<HyperGpu::GpuCmd *> &cmds,
				   const std::vector<HyperGpu::Semaphore *> &waitSemaphores,
				   const std::vector<HyperGpu::Semaphore *> &signalSemaphores,
				   HyperGpu::Fence *pFence) const;

protected:
	HyperGpu::GpuCmd*    m_pCmd           = nullptr;
	HyperGpu::Sampler*   m_pCommonSampler = nullptr;
	HyperGpu::GpuDevice* m_pGpuDevice     = nullptr;
	Area m_renderArea;
	HyperGpu::Fence *m_pRenderFence = nullptr;
	HyperGpu::Queue *m_pRenderQueue = nullptr;
	HyperGpu::Buffer* m_pGlobalBuffer = nullptr;

private:
	GlobalInfo m_globalInfo;
};

USING_RENDER_NAMESPACE_END

#endif //BASETOOL_H
