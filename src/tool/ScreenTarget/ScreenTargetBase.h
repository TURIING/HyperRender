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
#define FIGHT_FRAME_COUNT 3

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
        m_pPresentQueue = m_pGpuDevice->CreateQueue(HyperGpu::QueueType::Present);
        for(auto i = 0; i < FIGHT_FRAME_COUNT; i++) {
            m_vecScreenSemaphore.push_back(m_pGpuDevice->GetSyncManager()->CreateSemaphore());
        }
    }

    ~ScreenTargetBase() override {
        m_pCmd->SubRef();
        m_pScreenFence->SubRef();
        for (auto &semaphore: m_vecScreenSemaphore) {
            semaphore->SubRef();
        }
        m_pSurface->SubRef();
        m_pPresentQueue->SubRef();
        m_pGpuDevice->SubRef();
    }

    virtual void RenderToScreen(const DrawToTargetInfo& info) = 0;

protected:
    HyperGpu::GpuDevice* m_pGpuDevice = nullptr;
    HyperGpu::GpuCmd* m_pCmd = nullptr;
    HyperGpu::Fence* m_pScreenFence = nullptr;
    std::vector<HyperGpu::Semaphore*> m_vecScreenSemaphore;
    HyperGpu::GpuSurface* m_pSurface = nullptr;
    HyperGpu::Queue* m_pPresentQueue = nullptr;
};
#endif //SCREENTARGETBASE_H
