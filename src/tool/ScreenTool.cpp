/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/7 13:26
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "ScreenTool.h"
#include "ScreenTarget/ScreenWindowTarget.h"
#include <GpuDevice.h>

#include "../pass/ScreenPass.h"

USING_RENDER_NAMESPACE_BEGIN
ScreenTool::ScreenTool(GpuDevice* gpuDevice) : BaseTool(gpuDevice) {
    m_pRenderSemaphore = m_pGpuDevice->GetSyncManager()->CreateSemaphore();
}

ScreenTool::~ScreenTool() {
    m_pRenderSemaphore->SubRef();
    m_pScreenTexture->SubRef();
    m_pScreenTarget->SubRef();
}

IScreenTarget* ScreenTool::CreateScreen(const PlatformWindowInfo& platformSurfaceInfo) {
    return new ScreenWindowTarget(m_pGpuDevice, platformSurfaceInfo);
}

void ScreenTool::SetScreenTarget(IScreenTarget* target) {
    m_pScreenTarget = dynamic_cast<ScreenWindowTarget*>(target);
    m_pScreenTarget->AddRef();
}

void ScreenTool::BeginRenderToScreen(const Area& updateArea) {
    m_renderArea = updateArea;
    if (!m_pScreenTexture) {
        m_pScreenTexture = this->CreateDrawUnit(updateArea);
    }
    begin();
    static bool a = true;
    this->clearColor(m_pCmd, m_pScreenTexture, a ? Red: Blue);
    a = !a;
}

void ScreenTool::EndRenderToScreen() {
    end({m_pRenderSemaphore});
    renderToScreen();
}

void ScreenTool::renderToScreen() {
    ScreenWindowTarget::DrawToTargetInfo info{
        .pPresentSemaphore = &m_pRenderSemaphore,
        .semaphoreCount = 1,
        .pImage = m_pScreenTexture->GetImage(),
        .srcArea = m_renderArea,
        .dstArea = m_renderArea,
    };
    m_pScreenTarget->RenderToScreen(info);
}

USING_RENDER_NAMESPACE_END
