/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/24 20:32
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef SCREENTARGETBASE_H
#define SCREENTARGETBASE_H

#include "IScreenTarget.h"
#include "../../common/common.h"

class ScreenTargetBase : virtual public HyperRender::IScreenTarget {
public:
    struct DrawToTargetInfo {
        HyperGpu::Semaphore** pPresentSemaphore = nullptr;
        uint32_t semaphoreCount = 0;
        HyperGpu::Image2D* pImage = nullptr;
        HyperRender::Area srcArea;
        HyperRender::Area dstArea;
    };
public:
    explicit ScreenTargetBase(HyperGpu::GpuDevice* pGpuDevice): m_pGpuDevice(pGpuDevice) {
        m_pGpuDevice->AddRef();
        m_pCmd = pGpuDevice->GetCmdManager()->CreateCommandBuffer();
        m_pScreenFence = pGpuDevice->GetSyncManager()->CreateFence();
        m_pScreenSemaphore = pGpuDevice->GetSyncManager()->CreateSemaphore();
        m_pPresentQueue = m_pGpuDevice->CreateQueue(HyperGpu::QueueType::Present);
    }

    ~ScreenTargetBase() override {
        m_pCmd->SubRef();
        m_pScreenFence->SubRef();
        m_pScreenSemaphore->SubRef();
        m_pSurface->SubRef();
        m_pPresentQueue->SubRef();
        m_pGpuDevice->SubRef();
    }

    virtual void RenderToScreen(const DrawToTargetInfo& info) = 0;

protected:
    HyperGpu::GpuDevice* m_pGpuDevice = nullptr;
    HyperGpu::GpuCmd* m_pCmd = nullptr;
    HyperGpu::Fence* m_pScreenFence = nullptr;
    HyperGpu::Semaphore* m_pScreenSemaphore = nullptr;
    HyperGpu::GpuSurface* m_pSurface = nullptr;
    HyperGpu::Queue* m_pPresentQueue = nullptr;
};
#endif //SCREENTARGETBASE_H
