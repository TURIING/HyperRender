/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 15:29
* @version: 1.0
* @description: 
********************************************************************************/
#include "BaseTool.h"
#include "../common/Camera.h"

USING_RENDER_NAMESPACE_BEGIN
BaseTool::BaseTool(HyperGpu::GpuDevice* pGpuDevice) : m_pGpuDevice(pGpuDevice) {
    m_pGpuDevice->AddRef();
    m_pCmd           = m_pGpuDevice->GetCmdManager()->CreateCommandBuffer();
    m_pCommonSampler = m_pGpuDevice->GetResourceManager()->CreateSampler({});
    m_pRenderQueue   = m_pGpuDevice->CreateQueue(HyperGpu::QueueType::Graphics);
    m_pRenderFence   = m_pGpuDevice->GetSyncManager()->CreateFence();
}

BaseTool::~BaseTool() {
    m_pCommonSampler->SubRef();
    m_pGpuDevice->SubRef();
}

void BaseTool::ClearColor(IDrawUnit* targetUnit, HyperRender::Color color) {
	const auto unit = dynamic_cast<DrawUnit*>(targetUnit);

	m_pGpuDevice->GetCmdManager()->WithSingleCmdBuffer([&](HyperGpu::GpuCmd* pCmd) {
		pCmd->ClearColorImage(unit->GetImage(), std::bit_cast<HyperGpu::Color>(color));
    });
}

DrawUnit* BaseTool::CreateDrawUnit(const Area& area) {
    return new DrawUnit(m_pGpuDevice, {area, m_pCommonSampler});
}

void BaseTool::begin() const {
    m_pRenderFence->Reset();
    m_pCmd->Begin();
}

void BaseTool::end(const std::vector<HyperGpu::Semaphore*>& signalSemaphore) const {
    m_pCmd->End();
    this->SubmitCmd({m_pCmd}, {}, signalSemaphore, m_pRenderFence);
    m_pRenderFence->Wait();
}

void BaseTool::SubmitCmd(const std::vector<HyperGpu::GpuCmd*>&    cmds,
                         const std::vector<HyperGpu::Semaphore*>& waitSemaphores,
                         const std::vector<HyperGpu::Semaphore*>& signalSemaphores,
                         HyperGpu::Fence*                         pFence) const {
    HyperGpu::Queue::SubmitInfo info{
        .pWaitSemaphores = waitSemaphores.data(),
        .waitSemaphoreCount = TO_U32(waitSemaphores.size()),
        .pSignalSemaphores = signalSemaphores.data(),
        .signalSemaphoreCount = TO_U32(signalSemaphores.size()),
        .pFence = pFence,
        .pCmd = cmds.data(),
        .cmdCount = TO_U32(cmds.size())
    };
    m_pRenderQueue->Submit(info);
}

void BaseTool::clearColor(HyperGpu::GpuCmd* pCmd, IDrawUnit* targetUnit, HyperRender::Color color) {
    pCmd->ClearColorImage(dynamic_cast<DrawUnit*>(targetUnit)->GetImage(), std::bit_cast<HyperGpu::Color>(color));
}

USING_RENDER_NAMESPACE_END
