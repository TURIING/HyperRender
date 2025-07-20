/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/19 20:58
* @version: 1.0
* @description: 
********************************************************************************/
#include "ScreenWindowTarget.h"

USING_RENDER_NAMESPACE_BEGIN

ScreenWindowTarget::ScreenWindowTarget(HyperGpu::GpuDevice* pGpuDevice, const PlatformWindowInfo& info): ScreenTargetBase(pGpuDevice) {
    m_pSurface = m_pGpuDevice->CreateSurface(std::bit_cast<HyperGpu::PlatformWindowInfo>(info));
}

void ScreenWindowTarget::RenderToScreen(const DrawToTargetInfo& info) {
    m_pScreenFence->Wait();
    uint32_t imageIndex = 0;
    auto imageAvailableSemaphore = m_pSurface->AcquireNextImage(imageIndex);
    m_pScreenFence->Reset();
    
    m_pCmd->Begin();
    BEGIN_CMD_DEBUG_LABEL(m_pCmd, "ScreenWindowTarget::RenderToScreen");
    HyperGpu::ImageBlitRange range {
        .srcArea = std::bit_cast<HyperGpu::Area>(info.srcArea),
        .dstArea = std::bit_cast<HyperGpu::Area>(info.dstArea)
    };
    m_pCmd->BlitImageToSurface(info.pImage, m_pSurface, &range, 1, HyperGpu::Filter::LINEAR);
    END_CMD_DEBUG_LABEL(m_pCmd, "ScreenWindowTarget::RenderToScreen");
    m_pCmd->End();

    std::vector<HyperGpu::Semaphore*> waitSemaphore;
    waitSemaphore.reserve(info.semaphoreCount + 1);
    waitSemaphore.push_back(imageAvailableSemaphore);
    for (auto i = 0; i < info.semaphoreCount; i++) {
        waitSemaphore.push_back(info.pPresentSemaphore[i]);
    }
    HyperGpu::Queue::SubmitInfo submitInfo {
        .pWaitSemaphores = waitSemaphore.data(),
        .waitSemaphoreCount = TO_U32(waitSemaphore.size()),
        .pSignalSemaphores = &m_vecScreenSemaphore[imageIndex],
        .signalSemaphoreCount = 1,
        .pFence = m_pScreenFence,
        .pCmd = &m_pCmd,
        .cmdCount = 1
    };
    m_pPresentQueue->Submit(submitInfo);

    HyperGpu::Queue::PresentInfo presentInfo {
        .pWaitSemaphores = &m_vecScreenSemaphore[imageIndex],
        .waitSemaphoreCount = 1,
        .pSurface = m_pSurface,
        .imageIndex = &imageIndex,
    };
    m_pPresentQueue->Present(presentInfo);
}

USING_RENDER_NAMESPACE_END
