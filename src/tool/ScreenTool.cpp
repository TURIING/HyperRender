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

ScreenTool::ScreenTool(HyperGpu::GpuDevice* gpuDevice) : BaseTool(gpuDevice) {
    m_pRenderSemaphore = m_pGpuDevice->GetSyncManager()->CreateSemaphore();
    m_pScreenPass = new ScreenPass(gpuDevice);
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
    if (!m_pScreenTexture && !m_pTmpTexture) {
        m_pScreenTexture = this->CreateDrawUnit(updateArea);
        m_pTmpTexture = CreateDrawUnit(updateArea);
    }
    begin();
    static bool first = true;
    BaseTool::ClearColor(m_pTmpTexture, first ? Color::Red : Color::Green);
    first = !first;
    m_pScreenPass->SetScreenTexture(m_pTmpTexture->GetImage());

    auto image = m_pScreenTexture->GetImage();
    HyperGpu::GpuCmd::BeginRenderInfo beginInfo {
        .pPipeline = m_pScreenPass->GetPipeline(),
        .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
        .renderArea = std::bit_cast<HyperGpu::Area>(updateArea),
        .renderAttachmentType = HyperGpu::GpuCmd::RenderAttachmentType::Image2D,
        .renderAttachment = {1, &image},
    };
    m_pCmd->BeginRenderPass(beginInfo);
    m_pCmd->SetViewport({0, 0, (float)updateArea.size.width, (float)updateArea.size.height});
    m_pCmd->SetScissor({0, 0, updateArea.size.width, updateArea.size.height});
    m_pScreenPass->Draw(m_pCmd);
    m_pCmd->EndRenderPass();
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
