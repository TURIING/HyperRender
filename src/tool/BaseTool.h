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
public:
	explicit BaseTool(HyperGpu::GpuDevice* pGpuDevice);
	~BaseTool() override;

        void ClearColor(IDrawUnit *targetUnit, HyperRender::Color color) override;

        DrawUnit *CreateDrawUnit(const Area &area) override;

    protected:
        void clearColor(HyperGpu::GpuCmd *pCmd, IDrawUnit *targetUnit, HyperRender::Color color);

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
};

USING_RENDER_NAMESPACE_END

#endif //BASETOOL_H
