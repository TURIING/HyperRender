/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/1 22:33
* @version: 1.0
* @description: 
********************************************************************************/
#include "EffectTool.h"
#include "../pass/effect/RoundCornerPass.h"

EffectTool::EffectTool(HyperGpu::GpuDevice* pGpuDevice): BaseTool(pGpuDevice) {
    m_pRoundCornerPass = new RoundCornerPass(m_pGpuDevice);
    m_pImageAvailableSemaphore = m_pGpuDevice->GetSyncManager()->CreateSemaphore();
    m_pRenderFinishedSemaphore = m_pGpuDevice->GetSyncManager()->CreateSemaphore();
    m_pInFlightFence = m_pGpuDevice->GetSyncManager()->CreateFence();
}

EffectTool::~EffectTool() {
    m_pRoundCornerPass->SubRef();
    m_pImageAvailableSemaphore->SubRef();
    m_pRenderFinishedSemaphore->SubRef();
    m_pInFlightFence->SubRef();
    m_pCmd->SubRef();
}

void EffectTool::Begin(const BeginInfo& info) {
}

void EffectTool::SetRoundCorner(float radius) {
}

void EffectTool::End(IDrawUnit* resultUnit) {
}
